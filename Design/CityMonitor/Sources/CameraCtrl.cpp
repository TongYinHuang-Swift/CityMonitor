/**
 *=======================================================================================================================
 * Swift Technology ����˼ί���ؿƼ����޹�˾ ������Ƶ���ϵͳ���
 * Name        : CameraCtrl.cpp
 * Author      : ����
 * Version     : V1.0.0
 * Copyright   : �����������˼ί���ؿƼ����޹�˾������ӵ������Ȩ��������˼ί����������Ȩ��ɵ�����£��κ�������������
 *               ���öԱ��ļ����в��ֻ�ȫ���Ŀ��������򣬱���˾������׷����Ӧ�ķ������Ρ�
 * Description : ���������ģ��
 *=======================================================================================================================
 * Revision History:
 * �޸�ʱ��    �޸���  �޸�����
 * 2016-07-08  ����    �½�
 * 2016-07-xx  ����    ��ɰ汾V1.0.0
 *=======================================================================================================================
 */
#include "CameraCtrl.h"
#include "Debugger.h" 
#include "HCNetSDK.h"
#include "plaympeg4.h"

#ifdef WIN32
#else
#endif

extern void CALLBACK g_ExceptionCallBack(DWORD dwType, LONG lUserID, LONG lHandle, void *pUser);

CameraCtrl::CameraCtrl()
{

}
CameraCtrl::~CameraCtrl(void)
{
}

/***
 * desc:            ����ͷ��ʼ��
 * para:            None
 * retc:            None
 */
void CameraCtrl::Init(void)
{
    /* ��ʼ����������ͷSDK */
    NET_DVR_Init();
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
void CALLBACK CameraCtrl::RealDataCallBack_V30(LONG lRealHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, DWORD dwUser)
{
    HWND hWnd = GetConsoleWindow();

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

#if 0
void *CALLBACK CameraCtrl::ExceptionCallBack(DWORD dwType, LONG lUserID, LONG lHandle, void *pUser)
{
    char tempbuf[256] = {0};
    
    switch(dwType) 
    {
        case EXCEPTION_RECONNECT:    //Ԥ��ʱ����
        //PRINT(ALWAYS_PRINT, "SwiftHikSDK", __FUNCTION__, __LINE__,"Reconnect=%d",time(NULL));
        break;
        default:
        break;
    }
}
#endif


/***
 * desc:            ��¼����ָ��IP��ַ�������
 * pDVRIP:          ����ͷIP
 * wDVRPort:        ����ͷ�˿ڣ�Ĭ��8000
 * pUserName:       ��½�û���Ĭ��admin
 * pPassword:       ��½���룬Ĭ��admin
 * pDeviceInfo:     �豸��Ϣ
 * retc:            UserID--Success, -1--Error
 */
LONG CameraCtrl::Login(char *pDVRIP, WORD wDVRPort, char *pUserName, char *pPassword, LPNET_DVR_DEVICEINFO_V30 DeviceInfo)
{
    lUserID = NET_DVR_Login_V30( pDVRIP, wDVRPort, pUserName, pPassword, DeviceInfo );
    
    if ( lUserID < 0)
    {
        printf("Login error, %d\n", NET_DVR_GetLastError());
        NET_DVR_Cleanup();
        return -1;
    }
    else
    {
        printf("Login success!\n");
    }
    
    /* �����쳣��Ϣ�ص����� */
    NET_DVR_SetExceptionCallBack_V30(0, NULL, g_ExceptionCallBack, NULL);
    return lUserID;
}

/***
 * desc:            ��ȡ����ͷ�û�ID
 * para:            None
 * retc:            UserID
 */
LONG CameraCtrl::GetUsrID(void)
{
    return lUserID;
}

void CameraCtrl::SetPlayPort( LONG SetPort )
{
    lPort = SetPort;
}

/***
 * desc:            ����ͷ�ǳ�
 * para:            None
 * retc:            None
 */
void CameraCtrl::Exit(void)
{
    /* ע���û� */
    NET_DVR_Logout_V30(lUserID);
    NET_DVR_Cleanup();
}


