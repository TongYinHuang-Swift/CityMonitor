/**
 *=======================================================================================================================
 * Swift Technology ����˼ί���ؿƼ����޹�˾ ������Ƶ���ϵͳ���
 * Name        : MP4Player.cpp
 * Author      : ����
 * Version     : V1.0.0
 * Copyright   : �����������˼ί���ؿƼ����޹�˾������ӵ������Ȩ��������˼ί����������Ȩ��ɵ�����£��κ�������������
 *               ���öԱ��ļ����в��ֻ�ȫ���Ŀ��������򣬱���˾������׷����Ӧ�ķ������Ρ�
 * Description : �����MP4��ʽ��Ƶ������ģ��
 *=======================================================================================================================
 * Revision History:
 * �޸�ʱ��    �޸���  �޸�����
 * 2016-07-08  ����    �½�
 * 2016-07-xx  ����    ��ɰ汾V1.0.0
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
 * desc:            ��������ʼ��
 * para:            None
 * retc:            None
 */
void MP4Player::Init(void)
{
    PRINT(DEBUG_LEVEL_1, "MP4Player", __FUNCTION__, __LINE__);
}

/***
 * desc:            ����ʵʱԤ��
 * para:            None
 * retc:            None
 */
void MP4Player::RealPlayStart(void)
{
    PRINT(DEBUG_LEVEL_1, "MP4Player", __FUNCTION__, __LINE__);
    
    /* ����Ԥ�������ûص������� */
    NET_DVR_CLIENTINFO ClientInfo = {0};
    
    ClientInfo.hPlayWnd     = NULL;         /* ��ҪSDK����ʱ�����Ϊ��Чֵ����ȡ��������ʱ����Ϊ�� */
    ClientInfo.lChannel     = 1;            /* Ԥ��ͨ���� */
    ClientInfo.lLinkMode    = 0;            /* ���λ(31)Ϊ0��ʾ��������Ϊ1��ʾ������0��30λ��ʾ���ӷ�ʽ��0��TCP��ʽ��1��UDP��ʽ��2���ಥ��ʽ��3��RTP��ʽ; */
    ClientInfo.sMultiCastIP = NULL;         /* �ಥ��ַ����Ҫ�ಥԤ��ʱ���� */
    
    BOOL bPreviewBlock = false;             /* �������������Ƿ�������0����1���� */

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
 * desc:            ֹͣʵʱԤ��
 * para:            None
 * retc:            None
 */
void MP4Player::RealPlayStop(void)
{
    PRINT(DEBUG_LEVEL_1, "MP4Player", __FUNCTION__, __LINE__);
    /* �ر�Ԥ�� */
    NET_DVR_StopRealPlay(_g_lRealPlayHandle);
}


/***
 * desc:            //���ű����ļ�
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
        PRINT("��ȡͨ����ʧ��!");
        ErrorCode = -2;
        goto ERR_EXIT;
    }
    
    if ( !PlayM4_SetStreamOpenMode( lPort, STREAME_FILE) )  //����ʵʱ������ģʽ
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
    if ( !PlayM4_Play(lPort, hWnd) ) //���ſ�ʼ
    {
        PRINT("Start play failed!\n");
        ErrorCode = -5;
        goto ERR_EXIT;
    }
#else
    if ( !PlayM4_Play(lPort, NULL) ) //���ſ�ʼ
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
 * desc:            �˳����ű����ļ�
 * para:            None
 * retc:            None
 */
void MP4Player::PlayLocalFileExit(void)
{
    PlayM4_CloseFile(lPort);
}


/***
 * desc:            �����ٶȲ���
 * para:            None
 * retc:            None
 */
void MP4Player::Play(void)
{
    PRINT(DEBUG_LEVEL_9, "MP4Player", __FUNCTION__, __LINE__);
    
    PlayM4_Pause( lPort, PLAY_MODE_START );
} 

/***
 * desc:            ��ͣ����
 * para:            None
 * retc:            None
 */
void MP4Player::Pause(void)
{
    PRINT(DEBUG_LEVEL_1, "MP4Player", __FUNCTION__, __LINE__);
    PlayM4_Pause( lPort, PLAY_MODE_PAUSE );
}

/***
 * desc:            �ָ������ٶȲ���
 * para:            None
 * retc:            None
 */ 
void MP4Player::Resume(void)
{
    PRINT(DEBUG_LEVEL_1, "MP4Player", __FUNCTION__, __LINE__);
}

/***
 * desc:            ֹͣ����
 * para:            None
 * retc:            None
 */
void MP4Player::Stop(void)
{
    PRINT(DEBUG_LEVEL_1, "MP4Player", __FUNCTION__, __LINE__);
    PlayM4_Stop( lPort );
}

/***
 * desc:            ��ָ���ٶȿ��ٲ���
 * para:            None
 * retc:            None
 */
void MP4Player::PlayFast(void)
{
    PRINT(DEBUG_LEVEL_4, "MP4Player", __FUNCTION__, __LINE__);
    PlayM4_Fast( lPort );
}

/***
 * desc:            ��ָ���ٶ����ٲ���
 * para:            None
 * retc:            None
 */
void MP4Player::PlaySlow(void)
{
    PRINT(DEBUG_LEVEL_3, "MP4Player", __FUNCTION__, __LINE__);
    PlayM4_Slow( lPort );
}

/***
 * desc:            ����һ֡
 * para:            None
 * retc:            None
 */
void MP4Player::PlayOneFrame(void)
{
    PRINT(DEBUG_LEVEL_1, "MP4Player", __FUNCTION__, __LINE__);
    PlayM4_OneByOne( lPort );
}

/***
 * desc:            ÿ�벥��һ֡����������
 * para:            None
 * retc:            None
 */
void MP4Player::PlayOneByOne(void)
{
    PRINT(DEBUG_LEVEL_1, "MP4Player", __FUNCTION__, __LINE__);
}

/***
 * desc:            ��ָ���ٶȿ��ٵ���
 * para:            None
 * retc:            None
 */
void MP4Player::PlayFastBack(void)
{
    PRINT(DEBUG_LEVEL_4, "MP4Player", __FUNCTION__, __LINE__);
}

/***
 * desc:            ��ָ���ٶ����ٵ���
 * para:            None
 * retc:            None
 */
void MP4Player::PlaySlowBack(void)
{
    PRINT(DEBUG_LEVEL_3, "MP4Player", __FUNCTION__, __LINE__);
}

/***
 * desc:            ����һ֡
 * para:            None
 * retc:            None
 */
void MP4Player::PlayOneFrameBack(void)
{
    PRINT(DEBUG_LEVEL_1, "MP4Player", __FUNCTION__, __LINE__);
}

/***
 * desc:            ÿ�뵹��һ֡����������
 * para:            None
 * retc:            None
 */
void MP4Player::PlayOneByOneBack(void)
{
    PRINT(DEBUG_LEVEL_1, "MP4Player", __FUNCTION__, __LINE__);
}

