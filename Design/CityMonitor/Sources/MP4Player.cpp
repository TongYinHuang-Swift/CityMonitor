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
#include "CameraCtrl.h"

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

extern "C" {

void CALLBACK g_RealDataCallBack_V30(LONG lRealHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, DWORD dwUser)
{
    HWND hWnd = GetConsoleWindow();

    //CameraCtrl CamCtrl;
    LONG lPort = 0;

    if ( dwBufSize != 5120 )
    {
        printf("lRealHandle=%d, dwDataType=%d, dwBufSize=%d, dwUser=%d\n", lRealHandle, dwDataType, dwBufSize, dwUser);
        if ( pBuffer[1] == 0 )
        {
            int i = 0;
            for ( i = 0; i < 5; i++ )
            {
                
                printf("%2X ", pBuffer[i]);
            }
            system("PAUSE");
        }
    }

    switch (dwDataType)
    {
        case NET_DVR_SYSHEAD: //ϵͳͷ
        {
            if ( !PlayM4_GetPort(&lPort) )  //��ȡ���ſ�δʹ�õ�ͨ����
            {
                break;
            }
            //m_iPort = lPort; //��һ�λص�����ϵͳͷ������ȡ�Ĳ��ſ�port�Ÿ�ֵ��ȫ��port���´λص�����ʱ��ʹ�ô�port�Ų���
            if (dwBufSize > 0)
            {
                if ( !PlayM4_SetStreamOpenMode(lPort, STREAME_REALTIME) )  //����ʵʱ������ģʽ
                {
                    break;
                }
                
                if ( !PlayM4_OpenStream(lPort, pBuffer, dwBufSize, 1024 * 1024) ) //�����ӿ�
                {
                    break;
                }
                #ifdef _PLAY_ON_WINDOW
                if ( !PlayM4_Play(lPort, hWnd) ) //���ſ�ʼ
                {
                    break;
                }
                #else
                if ( !PlayM4_Play(lPort, NULL) ) //���ſ�ʼ
                {
                    break;
                }
                #endif
                
                #ifdef _PRINT_HEXDATA
                int i = 0;
                for ( i = 0; i < 32; i++ )
                {
                    if ( dwBufSize < 32 || pBuffer == NULL )
                    {
                        break;
                    }
                    printf("%d", pBuffer[i]);
                }
                #endif /* ENDIF _PRINT_HEXDATA */

                #ifdef _SAVE_H264_STREAM
                FILE*       fpsave;
                fpsave = fopen("test.record", "ab+");

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
        
        case NET_DVR_STREAMDATA:   //��������
        {
            if ( dwBufSize > 0 && lPort != -1 )
            {
                if ( !PlayM4_InputData(lPort, pBuffer, dwBufSize) )
                {
                    break;
                } 
                #ifdef _SAVE_H264_STREAM
                FILE*       fpsave;            // ��д��Ƶ�ļ��������ļ�ָ��
                fpsave = fopen("test.record", "ab+");

                if ( fpsave == NULL )
                {
                    printf("err:open file failed\n");
                    return;
                }
                fwrite(pBuffer, dwBufSize, 1, fpsave);
                fclose(fpsave);
                #endif
            }
        }
        
        default:
        break;
    }
    
}

void CALLBACK g_ExceptionCallBack(DWORD dwType, LONG lUserID, LONG lHandle, void *pUser)
{
    char tempbuf[256] = {0};

    printf("%s\n", __FUNCTION__ );
    
    switch(dwType) 
    {
        case EXCEPTION_RECONNECT:    //Ԥ��ʱ����
        //printf("----------reconnect--------%d\n", time(NULL));
        printf("----------reconnect--------\n");
        break;
        default:
        break;
    }
}

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
        printf("NET_DVR_RealPlay_V30 error\n");
        CamCtrl.Exit();
        return;
    }

    if ( !NET_DVR_SetRealDataCallBack( lRealPlayHandle, g_RealDataCallBack_V30, 0 ) )
    {
        printf("NET_DVR_SetRealDataCallBack error\n");
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
    NET_DVR_StopRealPlay(lRealPlayHandle);
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
        printf("File name error!\n");
        ErrorCode = -1;
        goto ERR_EXIT;
    }
    
    if ( !PlayM4_GetPort(&lPort) )
    {
        printf("��ȡͨ����ʧ��!");
        ErrorCode = -2;
        goto ERR_EXIT;
    }
    
    if ( !PlayM4_SetStreamOpenMode( lPort, STREAME_FILE) )  //����ʵʱ������ģʽ
    {
        printf("Set stream mode failed!\n");
        ErrorCode = -3;
        goto ERR_EXIT;
    }
    
    if ( !PlayM4_OpenFile(lPort, sFileName ) )
    {
        printf("Open stream file failed!\n");
        ErrorCode = -4;
        goto ERR_EXIT;
    }
       
#ifdef WIN32
    if ( !PlayM4_Play(lPort, hWnd) ) //���ſ�ʼ
    {
        printf("Start play failed!\n");
        ErrorCode = -5;
        goto ERR_EXIT;
    }
#else
    if ( !PlayM4_Play(lPort, NULL) ) //���ſ�ʼ
    {
        printf("Start play failed!\n");
        ErrorCode = -6;
        goto ERR_EXIT;
    }
#endif

ERR_EXIT:
#ifdef WIN32
        if (ErrorCode)
        {
            system("PAUSE");
        }
#endif
        return ErrorCode;
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

