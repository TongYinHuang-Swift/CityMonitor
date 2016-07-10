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
#include <time.h>
#include "MP4Player.h"
#include "Debugger.h" 

#ifdef WIN32
#else
#endif

#ifndef _SAVE_H264_STREAM
#define _SAVE_H264_STREAM
#endif

static int _g_playOnWinEn = 0;
static LONG _g_lPort = 0;           //ȫ�ֵĲ��ſ�port��

MP4Player::MP4Player()
{
    //PRINT(DEBUG_LEVEL_1, "MP4Player", __FUNCTION__, __LINE__);
}
MP4Player::~MP4Player()
{
    //PRINT(DEBUG_LEVEL_1, "MP4Player", __FUNCTION__, __LINE__);
}

/***
 * desc:            ��������ʼ��
 * para:            None
 * retc:            None
 */
void MP4Player::Init(void)
{
    PRINT(DEBUG_LEVEL_1, "MP4Player", __FUNCTION__, __LINE__);
    this->pSelectedCamera = NULL;
    this->pSelectedRealVideoBuffer = NULL;
    this->SaveDataInit();
}

void MP4Player::SetCameraCtrl(CameraCtrl *pCameraCtrl)
{
    this->pSelectedCamera = pCameraCtrl;
}

void MP4Player::SetRealVideoBuffer(Buffer *realVideoBuffer)
{
    this->pSelectedRealVideoBuffer = realVideoBuffer;
}

void MP4Player::SaveData(Buffer *pBuffer, BYTE *pRcvData, int dataLen )
{
    while (0 != isTerminted)
    {
        int len = dataLen;
        if (len > 0)
        {
            unsigned int i = 0;
            int result = 0;
            
            PRINT(ALWAYS_PRINT, "Receiver", __FUNCTION__, __LINE__, "Received Data=%d", len);
            
            while ( len > 0 )
            {
                // ��ѯѭ��������ʣ��ռ�
                uint writelen = 0;
                uint availLen = pBuffer->GetUnusedSpace();
                if (availLen < len)
                {
                    writelen = availLen;
                }
                else
                {
                    writelen = len;
                }

                if (writelen > 0)
                {
                    // ѭ�����������ο�д��availLen���ֽ�
                    result = pBuffer->Write(pRcvData + i, writelen); // д������
                    
                    PRINT(ALWAYS_PRINT, "Receiver", __FUNCTION__, __LINE__, "write len=%d,%d",writelen,result);

                    len -= writelen;                            // �����´ο�д����ֽ���
                    i += writelen;                         // ��������Դ��ַָ��
                }
                else
                {
#ifdef WIN32
                    Sleep(100);
#else
                    usleep(100000);
#endif
                }
            }
        }
        else
        {
#ifdef WIN32
            lastError = WSAGetLastError();

            switch (lastError)
            {
            case 0:                 // The connection has been gracefully closed
                break;
            case WSAENETDOWN:       // The network subsystem has failed
                break;
            case WSAEFAULT:         // The buffer parameter is not completely contained in a valid part of the user address space
                break;
            case WSAENOTCONN:       // The socket is not connected
                break;
            case WSAEINTR:          // The socket was closed
                break;
            case WSAENETRESET:      // The connection has been broken due to the keep-alive activity detecting a failure while the operation was in progress
                break;
            case WSAESHUTDOWN:      // The socket has been shut down; it is not possible to receive on a socket after shutdown has been invoked with how set to SD_RECEIVE or SD_BOTH
                break;
            case WSAEWOULDBLOCK:    // The socket is marked as nonblocking and the receive operation would block
                break;
            case WSAEMSGSIZE:       // The message was too large to fit into the specified buffer and was truncated
                break;
            case WSAECONNABORTED:   // The virtual circuit was terminated due to a time-out or other failure. The application should close the socket as it is no longer usable
                break;
            case WSAETIMEDOUT:      // The connection has been dropped because of a network failure or because the peer system failed to respond
                break;
            case WSAECONNRESET:     // The virtual circuit was reset by the remote side executing a hard or abortive close. The application should close the socket because it is no longer usable
                break;
            case 10038:
                PRINT(ALWAYS_PRINT, "Receiver", __FUNCTION__, __LINE__, "receiveerror: %d %s\n",   lastError,"��һ�����׽����ϳ�����һ������");
                break;
            }

            if (0 != lastError)
            {
                PRINT(ALWAYS_PRINT, "Receiver", __FUNCTION__, __LINE__, "receiveerror: %d \n",   lastError);
            }
#else
            lastError = errno;
            if (0 != lastError)
            {
                PRINT(ALWAYS_PRINT, "BasicTCP", __FUNCTION__, __LINE__, "receiveerror: %s \n",   strerror(errno));
            }
#endif
        }
    }
}

void MP4Player::SaveDataInit(void)
{
    this->isTerminted = 0;
    this->lastError = 0;
}

void MP4Player::SaveDataExit(void)
{
    this->isTerminted = 1;
    this->lastError = 0;
}

void MP4Player::SaveRealVideoData(BYTE *pRcvData, int dataLen)
{
    if (!this->pSelectedRealVideoBuffer)
    this->SaveData(this->pSelectedRealVideoBuffer, pRcvData, dataLen);
}

void MP4Player::SaveToFile(BYTE *pBuffer, DWORD dwBufSize)
{
    FILE* fpsave = fopen("test.record", "ab+");

    if ( fpsave == NULL )
    {
        printf("err:open file failed\n");
        return;
    }
    fwrite(pBuffer, dwBufSize, 1, fpsave);
    
    fclose(fpsave);
}

extern "C" void CALLBACK CCallRealDataCallBack_V30(LONG lRealHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, DWORD dwUser )
{
    MP4Player* pFunc = new MP4Player();
    pFunc->RealDataCallBack_V30( lRealHandle, dwDataType, pBuffer, dwBufSize, dwUser);
    delete pFunc;
}

/***
 * desc:            ����ͷ��Ƶ���ص�����
 * lRealHandle:
 * dwDataType:
 * pBuffer: 
 * dwBufSize:
 * dwUser: 
 * retc:            None
 */
void CALLBACK MP4Player::RealDataCallBack_V30(LONG lRealHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, DWORD dwUser)
{
    /* ʵʱ������ */
    this->PlayRealData(dwDataType, pBuffer, dwBufSize);     /* ʵʱԤ�� */
    return ;

    this->SaveRealVideoData(pBuffer, dwBufSize);            /* ���� */
}

void MP4Player::PlayRealData(DWORD dwDataType,BYTE *pBuffer, DWORD dwBufSize)
{
    HWND hWnd = NULL;
#ifdef WIN32
    if ( this->GetPlayOnWindow() == DISPLAY_ENABLE )
    {
         hWnd = GetConsoleWindow();
    }
#endif

    switch (dwDataType)
    {
        case NET_DVR_SYSHEAD:               /* ϵͳͷ */
        {
            if ( !PlayM4_GetPort(&_g_lPort) )  /* ��ȡ���ſ�δʹ�õ�ͨ���� */
            {
                break;
            }
            
            /* ��һ�λص�����ϵͳͷ������ȡ�Ĳ��ſ�port�Ÿ�ֵ��ȫ��port���´λص�����ʱ��ʹ�ô�port�Ų��� */
            
            if (dwBufSize > 0)
            {
                if ( !PlayM4_SetStreamOpenMode(_g_lPort, STREAME_REALTIME) )  /* ����ʵʱ������ģʽ */
                {
                    break;
                }
                
                if ( !PlayM4_OpenStream(_g_lPort, pBuffer, dwBufSize, 1024 * 1024) ) /* �����ӿ� */
                {
                    break;
                }
                
                if ( !PlayM4_Play(_g_lPort, hWnd) ) /* ���ſ�ʼ */
                {
                    break;
                }
            }
        }
        /* No break */
        
        case NET_DVR_STREAMDATA:   /* �������� */
        {
            if ( dwBufSize > 0 && _g_lPort != -1 )
            {
                if ( !PlayM4_InputData(_g_lPort, pBuffer, dwBufSize) )
                {
                    break;
                }
            }
        }
        /* No break */
        
        default:
        break;
    }
}


/***
 * desc:            ����ʵʱԤ��
 * para:            None
 * retc:            None
 */
void MP4Player::RealPlayInit(LONG lUserID)
{
    PRINT(DEBUG_LEVEL_1, "MP4Player", __FUNCTION__, __LINE__);
    
    /* ����Ԥ�������ûص������� */
    NET_DVR_CLIENTINFO ClientInfo = {0};
    
    ClientInfo.hPlayWnd     = NULL;         /* ��ҪSDK����ʱ�����Ϊ��Чֵ����ȡ��������ʱ����Ϊ�� */
    ClientInfo.lChannel     = 1;            /* Ԥ��ͨ���� */
    ClientInfo.lLinkMode    = 0;            /* ���λ(31)Ϊ0��ʾ��������Ϊ1��ʾ������0��30λ��ʾ���ӷ�ʽ��0��TCP��ʽ��1��UDP��ʽ��2���ಥ��ʽ��3��RTP��ʽ; */
    ClientInfo.sMultiCastIP = NULL;         /* �ಥ��ַ����Ҫ�ಥԤ��ʱ���� */
    
    BOOL bPreviewBlock = false;             /* �������������Ƿ�������0����1���� */
    
    lRealPlayHandle = NET_DVR_RealPlay_V30( lUserID, &ClientInfo, NULL, NULL, 0 );

    if (lRealPlayHandle < 0)
    {
        printf("NET_DVR_RealPlay_V30 error\n");
        pSelectedCamera->Exit();
        return;
    }
    
    if ( !NET_DVR_SetRealDataCallBack( lRealPlayHandle, CCallRealDataCallBack_V30, 0 ) )
    {
        printf("NET_DVR_SetRealDataCallBack error\n");
    }
}


/***
 * desc:            ֹͣʵʱԤ��
 * para:            None
 * retc:            None
 */
void MP4Player::RealPlayExit(void)
{
    PRINT(DEBUG_LEVEL_1, "MP4Player", __FUNCTION__, __LINE__);
    /* �ر�Ԥ�� */
    NET_DVR_StopRealPlay(lRealPlayHandle);
}

void MP4Player::SetPlayOnWindow(int playOnWin)
{
    _g_playOnWinEn = playOnWin;
}

int MP4Player::GetPlayOnWindow(void)
{
    return _g_playOnWinEn;
}


/***
 * desc:            ���ű����ļ�
 * para:            None
 * retc:            �ɹ�--���ض˿ںţ�ʧ��--���ش�����
 */
LONG MP4Player::PlayLocalFile( LPSTR sFileName )
{
    HWND hWnd = NULL;
#ifdef WIN32
    if ( this->GetPlayOnWindow() == DISPLAY_ENABLE )
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
        printf("��ȡͨ����ʧ��!");
        ErrorCode = ERR_GET_PLAY_PORT_FAIL;
        goto ERR_EXIT;
    }
    
    if ( !PlayM4_SetStreamOpenMode( lPort, STREAME_FILE) )  //����ʵʱ������ģʽ
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
    
    if ( !PlayM4_Play(lPort, hWnd) ) //���ſ�ʼ
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
 * desc:            �˳����ű����ļ�
 * para:            None
 * retc:            None
 */
void MP4Player::PlayLocalFileExit(void)
{
    PlayM4_CloseFile(lPort);
}

/***
 * desc:            ���ز��ſ��ƹ��ܲ���
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

