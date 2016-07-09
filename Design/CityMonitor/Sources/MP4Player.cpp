/**
 *=======================================================================================================================
 * Swift Technology 重庆思委夫特科技有限公司 城市视频监控系统软件
 * Name        : MP4Player.cpp
 * Author      : 董超
 * Version     : V1.0.0
 * Copyright   : 本软件由重庆思委夫特科技有限公司开发并拥有所有权利，在无思委夫特书面授权许可的情况下，任何其他团体或个人
 *               不得对本文件进行部分或全部的拷贝，否则，本公司将依法追究相应的法律责任。
 * Description : 摄像机MP4格式视频播放器模块
 *=======================================================================================================================
 * Revision History:
 * 修改时间    修改人  修改内容
 * 2016-07-08  董超    新建
 * 2016-07-xx  董超    完成版本V1.0.0
 *=======================================================================================================================
 */
#include "MP4Player.h"
#include "Debugger.h" 

#ifdef WIN32
#else
#endif

MP4Player::MP4Player()
{
    PRINT(DEBUG_LEVEL_1, "MP4Player", __FUNCTION__, __LINE__);
}
MP4Player::~MP4Player()
{
    PRINT(DEBUG_LEVEL_1, "MP4Player", __FUNCTION__, __LINE__);
}

/***
 * desc:            播放器初始化
 * para:            None
 * retc:            None
 */
void MP4Player::Init(void)
{
    PRINT(DEBUG_LEVEL_1, "MP4Player", __FUNCTION__, __LINE__);
}

/***
 * desc:            启动实时预览
 * para:            None
 * retc:            None
 */
void MP4Player::RealPlayStart(void)
{
    PRINT(DEBUG_LEVEL_1, "MP4Player", __FUNCTION__, __LINE__);
    
    /* 启动预览并设置回调数据流 */
    NET_DVR_CLIENTINFO ClientInfo = {0};
    
    ClientInfo.hPlayWnd     = NULL;         /* 需要SDK解码时句柄设为有效值，仅取流不解码时可设为空 */
    ClientInfo.lChannel     = 1;            /* 预览通道号 */
    ClientInfo.lLinkMode    = 0;            /* 最高位(31)为0表示主码流，为1表示子码流0～30位表示连接方式：0－TCP方式；1－UDP方式；2－多播方式；3－RTP方式; */
    ClientInfo.sMultiCastIP = NULL;         /* 多播地址，需要多播预览时配置 */
    
    BOOL bPreviewBlock = false;             /* 请求码流过程是否阻塞，0：否，1：是 */

    CameraCtrl CamCtrl;
    
    lUserID = CamCtrl.GetUsrID();
    
    lRealPlayHandle = NET_DVR_RealPlay_V30( lUserID, &ClientInfo, NULL, NULL, 0 );

    if (lRealPlayHandle < 0)
    {
        PRINT("NET_DVR_RealPlay_V30 error\n");
        CamCtrl.Exit();
        return;
    }

    if ( !NET_DVR_SetRealDataCallBack( lRealPlayHandle, g_RealDataCallBack_V30, 0 ) )
    {
        PRINT("NET_DVR_SetRealDataCallBack error\n");
    }
}


/***
 * desc:            停止实时预览
 * para:            None
 * retc:            None
 */
void MP4Player::RealPlayStop(void)
{
    PRINT(DEBUG_LEVEL_1, "MP4Player", __FUNCTION__, __LINE__);
    /* 关闭预览 */
    NET_DVR_StopRealPlay(_g_lRealPlayHandle);
}


/***
 * desc:            //播放本地文件
 * para:            None
 * retc:            None
 */
int MP4Player::PlayLocalFile( LPSTR sFileName )
{
#ifdef WIN32
    HWND hWnd = GetConsoleWindow();
#endif

    int ErrorCode = 0;

    if ( !sFileName )
    {
        PRINT("File name error!\n");
        ErrorCode = -1;
        goto ERR_EXIT;
    }
    
    if ( !PlayM4_GetPort(&lPort) )
    {
        PRINT("获取通道号失败!");
        ErrorCode = -2;
        goto ERR_EXIT;
    }
    
    if ( !PlayM4_SetStreamOpenMode( lPort, STREAME_FILE) )  //设置实时流播放模式
    {
        PRINT("Set stream mode failed!\n");
        ErrorCode = -3;
        goto ERR_EXIT;
    }
    
    if ( !PlayM4_OpenFile(lPort, sFileName ) )
    {
        PRINT("Open stream file failed!\n");
        ErrorCode = -4;
        goto ERR_EXIT;
    }
       
#ifdef WIN32
    if ( !PlayM4_Play(lPort, hWnd) ) //播放开始
    {
        PRINT("Start play failed!\n");
        ErrorCode = -5;
        goto ERR_EXIT;
    }
#else
    if ( !PlayM4_Play(lPort, NULL) ) //播放开始
    {
        PRINT("Start play failed!\n");
        ErrorCode = -6;
        goto ERR_EXIT;
    }

ERR_EXIT:
#ifdef WIN32
        system("PAUSE");
#endif
        return ErrorCode;
#endif
}


/***
 * desc:            退出播放本地文件
 * para:            None
 * retc:            None
 */
void MP4Player::PlayLocalFileExit(void)
{
    PlayM4_CloseFile(lPort);
}


/***
 * desc:            正常速度播放
 * para:            None
 * retc:            None
 */
void MP4Player::Play(void)
{
    PRINT(DEBUG_LEVEL_9, "MP4Player", __FUNCTION__, __LINE__);
    
    PlayM4_Pause( lPort, PLAY_MODE_START );
} 

/***
 * desc:            暂停播放
 * para:            None
 * retc:            None
 */
void MP4Player::Pause(void)
{
    PRINT(DEBUG_LEVEL_1, "MP4Player", __FUNCTION__, __LINE__);
    PlayM4_Pause( lPort, PLAY_MODE_PAUSE );
}

/***
 * desc:            恢复正常速度播放
 * para:            None
 * retc:            None
 */ 
void MP4Player::Resume(void)
{
    PRINT(DEBUG_LEVEL_1, "MP4Player", __FUNCTION__, __LINE__);
}

/***
 * desc:            停止播放
 * para:            None
 * retc:            None
 */
void MP4Player::Stop(void)
{
    PRINT(DEBUG_LEVEL_1, "MP4Player", __FUNCTION__, __LINE__);
    PlayM4_Stop( lPort );
}

/***
 * desc:            按指定速度快速播放
 * para:            None
 * retc:            None
 */
void MP4Player::PlayFast(void)
{
    PRINT(DEBUG_LEVEL_4, "MP4Player", __FUNCTION__, __LINE__);
    PlayM4_Fast( lPort );
}

/***
 * desc:            按指定速度慢速播放
 * para:            None
 * retc:            None
 */
void MP4Player::PlaySlow(void)
{
    PRINT(DEBUG_LEVEL_3, "MP4Player", __FUNCTION__, __LINE__);
    PlayM4_Slow( lPort );
}

/***
 * desc:            播放一帧
 * para:            None
 * retc:            None
 */
void MP4Player::PlayOneFrame(void)
{
    PRINT(DEBUG_LEVEL_1, "MP4Player", __FUNCTION__, __LINE__);
    PlayM4_OneByOne( lPort );
}

/***
 * desc:            每秒播放一帧，连续播放
 * para:            None
 * retc:            None
 */
void MP4Player::PlayOneByOne(void)
{
    PRINT(DEBUG_LEVEL_1, "MP4Player", __FUNCTION__, __LINE__);
}

/***
 * desc:            按指定速度快速倒退
 * para:            None
 * retc:            None
 */
void MP4Player::PlayFastBack(void)
{
    PRINT(DEBUG_LEVEL_4, "MP4Player", __FUNCTION__, __LINE__);
}

/***
 * desc:            按指定速度慢速倒退
 * para:            None
 * retc:            None
 */
void MP4Player::PlaySlowBack(void)
{
    PRINT(DEBUG_LEVEL_3, "MP4Player", __FUNCTION__, __LINE__);
}

/***
 * desc:            倒退一帧
 * para:            None
 * retc:            None
 */
void MP4Player::PlayOneFrameBack(void)
{
    PRINT(DEBUG_LEVEL_1, "MP4Player", __FUNCTION__, __LINE__);
}

/***
 * desc:            每秒倒退一帧，连续播放
 * para:            None
 * retc:            None
 */
void MP4Player::PlayOneByOneBack(void)
{
    PRINT(DEBUG_LEVEL_1, "MP4Player", __FUNCTION__, __LINE__);
}

