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
#include <time.h>
#include "MP4Player.h"
#include "Debugger.h" 
#include "CameraCtrl.h"

#ifdef WIN32
#else
#endif

#ifndef _SAVE_H264_STREAM
#define _SAVE_H264_STREAM
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

#ifdef __cplusplus
extern "C" {
#endif
#if 0
/***
 * desc:            实时流回调函数
 * lRealHandle:     
 * dwDataType: 
 * pBuffer:         流数据缓存
 * dwBufSize:       缓存大小
 * dwUser:          用户
 * retc:            None
 */
void CALLBACK g_RealDataCallBack_V30(LONG lRealHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, DWORD dwUser)
{
    HWND hWnd = NULL;
#ifdef WIN32 
    hWnd = GetConsoleWindow();
#else
    hWnd = NULL;
#endif

    LONG lPort = 0;

    switch (dwDataType)
    {
        case NET_DVR_SYSHEAD:               /* 系统头 */
        {
            if ( !PlayM4_GetPort(&lPort) )  /* 获取播放库未使用的通道号 */
            {
                break;
            }
            
            /* 第一次回调的是系统头，将获取的播放库port号赋值给全局port，下次回调数据时即使用此port号播放 */
            
            if (dwBufSize > 0)
            {
                if ( !PlayM4_SetStreamOpenMode(lPort, STREAME_REALTIME) )  /* 设置实时流播放模式 */
                {
                    break;
                }
                
                if ( !PlayM4_OpenStream(lPort, pBuffer, dwBufSize, 1024 * 1024) ) /* 打开流接口 */
                {
                    break;
                }
                
                if ( !PlayM4_Play(lPort, hWnd) ) /* 播放开始 */
                {
                    break;
                }
                
#ifdef _SAVE_H264_STREAM
                FILE*       fpsave = fopen("test.record", "ab+");

                if ( fpsave == NULL )
                {
                    printf("err:open file failed\n");
                    return;
                }
                fwrite(pBuffer, dwBufSize, 1, fpsave);
                fclose(fpsave);
#endif /* ENDIF _SAVE_H264_STREAM */
            }
        }
        /* No break */
        
        case NET_DVR_STREAMDATA:   /* 码流数据 */
        {
            if ( dwBufSize > 0 && lPort != -1 )
            {
                if ( !PlayM4_InputData(lPort, pBuffer, dwBufSize) )
                {
                    break;
                } 
                
#ifdef _SAVE_H264_STREAM
                FILE*       fpsave = fopen("test.record", "ab+");
                if ( fpsave == NULL )
                {
                    printf("err:open file failed\n");
                    return;
                }
                fwrite(pBuffer, dwBufSize, 1, fpsave);
                fclose(fpsave);
#endif /* ENDIF _SAVE_H264_STREAM */
            }
        }
        /* No break */
        
        default:
        break;
    }
}

/***
 * desc:            异常消息回调函数
 * dwType:
 * lUserID:         
 * lHandle:
 * pUser:           
 * retc:            None
 */
void CALLBACK g_ExceptionCallBack(DWORD dwType, LONG lUserID, LONG lHandle, void *pUser)
{
    printf("%s\n", __FUNCTION__ );
    
    switch (dwType) 
    {
        case EXCEPTION_RECONNECT:    /* 预览时重连 */
        {
            printf("----------reconnect--------%d\n", time(NULL));
        }
        break;
        
        default:
        break;
    }
}
#endif
#ifdef __cplusplus
    }
#endif /* ENDIF __cplusplus */


/***
 * desc:            启动实时预览
 * para:            None
 * retc:            None
 */
void MP4Player::RealPlayStart(LONG lUserID)
{
    PRINT(DEBUG_LEVEL_1, "MP4Player", __FUNCTION__, __LINE__);
    
    /* 启动预览并设置回调数据流 */
    NET_DVR_CLIENTINFO ClientInfo = {0};
    
    ClientInfo.hPlayWnd     = NULL;         /* 需要SDK解码时句柄设为有效值，仅取流不解码时可设为空 */
    ClientInfo.lChannel     = 1;            /* 预览通道号 */
    ClientInfo.lLinkMode    = 0;            /* 最高位(31)为0表示主码流，为1表示子码流0～30位表示连接方式：0－TCP方式；1－UDP方式；2－多播方式；3－RTP方式; */
    ClientInfo.sMultiCastIP = NULL;         /* 多播地址，需要多播预览时配置 */
    
    BOOL bPreviewBlock = false;             /* 请求码流过程是否阻塞，0：否，1：是 */
    
    lRealPlayHandle = NET_DVR_RealPlay_V30( lUserID, &ClientInfo, NULL, NULL, 0 );

    if (lRealPlayHandle < 0)
    {
        printf("NET_DVR_RealPlay_V30 error\n");
        CameraCtrl *CamCtrl = new CameraCtrl();
        CamCtrl->Exit();
        delete CamCtrl;
        return;
    }

#if 0
    if ( !NET_DVR_SetRealDataCallBack( lRealPlayHandle, g_RealDataCallBack_V30, 0 ) )
    {
        printf("NET_DVR_SetRealDataCallBack error\n");
    }
#else
    if ( !NET_DVR_SetRealDataCallBack( lRealPlayHandle, CCallRealDataCallBack_V30, 0 ) )
    {
        printf("NET_DVR_SetRealDataCallBack error\n");
    }
#endif
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
    NET_DVR_StopRealPlay(lRealPlayHandle);
}

void MP4Player::SetPlayOnWindow(PLAY_ON_WINDOW_EN_T playOnWin)
{
    playOnWinEn = playOnWin;
}


/***
 * desc:            播放本地文件
 * para:            None
 * retc:            成功--返回端口号；失败--返回错误码
 */
LONG MP4Player::PlayLocalFile( LPSTR sFileName )
{
    HWND hWnd = NULL;
#ifdef WIN32
    if ( this->playOnWinEn == DISPLAY_ENABLE )
    {
         hWnd = GetConsoleWindow();
    }
#endif

    int ErrorCode = 0;

    if ( !sFileName )
    {
        printf("File name error!\n");
        ErrorCode = ERR_FILE_NAME_INVALID;
        goto ERR_EXIT;
    }
    this->sHistoryFileName = sFileName;
    
    if ( !PlayM4_GetPort(&lPort) )
    {
        printf("获取通道号失败!");
        ErrorCode = ERR_GET_PLAY_PORT_FAIL;
        goto ERR_EXIT;
    }
    
    if ( !PlayM4_SetStreamOpenMode( lPort, STREAME_FILE) )  //设置实时流播放模式
    {
        printf("Set stream mode failed!\n");
        ErrorCode = ERR_SET_STREAM_OPEN_MODE_FAIL;
        goto ERR_EXIT;
    }
    
    if ( !PlayM4_OpenFile(lPort, sFileName ) )
    {
        printf("Open stream file failed!\n");
        ErrorCode = ERR_OPEN_FILE_FAIL;
        goto ERR_EXIT;
    }
    
    if ( !PlayM4_Play(lPort, hWnd) ) //播放开始
    {
        printf("Start play failed!\n");
        ErrorCode = ERR_START_PLAY_FAIL;
        goto ERR_EXIT;
    }

ERR_EXIT:
#ifdef WIN32
        if (ErrorCode)
        {
            system("PAUSE");
            return ErrorCode;
        }
        else
        {
            return lPort;
        }
#else
        if (ErrorCode)
        {
            pause();
        }
        {
            return lPort;
        }
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
 * desc:            本地播放控制功能测试
 * para:            None
 * retc:            None
 */
void MP4Player::PlayLocalFileCtrl(void)
{
    int ctrl_type = 0;
    this->speedChangeVal = 0;

    while (1)
    {
        printf("Input play ctrl type:\n");
        scanf("%d", &ctrl_type);
        
        switch (ctrl_type)
        {
            case SW_HIK_PLAY_START:
            {
                printf("Play start!\n");
                PlayM4_Pause( this->lPort, PLAY_MODE_START );
            }
            break;

            case SW_HIK_PLAY_PAUSE:
            {
                printf("Play pause!\n");
                PlayM4_Pause( this->lPort, PLAY_MODE_PAUSE );
            }
            break;

            case SW_HIK_PLAY_STOP:
            {
                printf("Play stop!\n");
                PlayM4_Stop( this->lPort );
            }
            break;

            case SW_HIK_PLAY_RESUME:
            {
                printf("Play resume!\n");
                this->speedChangeVal = 0;
                this->PlayLocalFileExit();
                this->PlayLocalFile(this->sHistoryFileName);
            }
            break;

            case SW_HIK_PLAY_SLOW_FWD:
            {
                printf("Play slow fwd!\n");
                PlayM4_Slow(this->lPort);
                this->speedChangeVal--;
            }
            break;

            case SW_HIK_PLAY_FAST_FWD:
            {
                printf("Play fast fwd!\n");
                PlayM4_Fast(this->lPort);
                this->speedChangeVal++;
            }
            break;

            case SW_HIK_PLAY_ONE_BY_ONE:
            {
                printf("Play one by one!\n");
                this->speedChangeVal = 0;
                PlayM4_OneByOne(this->lPort);
            }
            break;

            case SW_HIK_PLAY_SLOW_2X:
            {
                printf("Play slow! 2X\n");
                PlayM4_Slow(this->lPort);
                Sleep(10);
                PlayM4_Slow(this->lPort);
                this->speedChangeVal -= 2;
            }
            break;

            case SW_HIK_PLAY_SLOW_4X:
            {
                printf("Play slow! 4X\n");
                PlayM4_Slow(this->lPort);
                Sleep(10);
                PlayM4_Slow(this->lPort);
                Sleep(10);
                PlayM4_Slow(this->lPort);
                Sleep(10);
                PlayM4_Slow(this->lPort);
                this->speedChangeVal -= 4;
            }
            break;

            case SW_HIK_PLAY_FAST_2X:
            {
                printf("Play fast! 2X\n");
                PlayM4_Fast(this->lPort);
                Sleep(10);
                PlayM4_Fast(this->lPort);
                this->speedChangeVal += 2;
            }
            break;

            case SW_HIK_PLAY_FAST_4X:
            {
                printf("Play fast! 4X\n");
                PlayM4_Fast(this->lPort);
                Sleep(10);
                PlayM4_Fast(this->lPort);
                Sleep(10);
                PlayM4_Fast(this->lPort);
                Sleep(10);
                PlayM4_Fast(this->lPort);
                this->speedChangeVal += 4;
            }
            break;
 
            default:
            {
                if ( this->speedChangeVal >= 0 )
                {
                    while (this->speedChangeVal--)
                    {
                        PlayM4_Slow(this->lPort);
                        Sleep(2);
                    }
                }
                else
                {
                    while (this->speedChangeVal++)
                    {
                        PlayM4_Fast(this->lPort);
                        Sleep(2);
                    }
                }
            }
            break;
        }
        Sleep(10);
    }
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

