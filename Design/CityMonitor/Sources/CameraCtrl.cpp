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
#include <time.h>
#include "CameraCtrl.h"
#include "Debugger.h" 
#include "HCNetSDK.h"
#include "plaympeg4.h"
#include "CameraCtrl.h"
#include "MP4Player.h"

#ifdef WIN32
#else
#endif

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
    HWND hWnd = NULL;
#ifdef WIN32 
    hWnd = GetConsoleWindow();
#else
    hWnd = NULL;
#endif

    LONG lPort = 0;

    switch (dwDataType)
    {
        case NET_DVR_SYSHEAD:               /* ϵͳͷ */
        {
            if ( !PlayM4_GetPort(&lPort) )  /* ��ȡ���ſ�δʹ�õ�ͨ���� */
            {
                break;
            }
            
            /* ��һ�λص�����ϵͳͷ������ȡ�Ĳ��ſ�port�Ÿ�ֵ��ȫ��port���´λص�����ʱ��ʹ�ô�port�Ų��� */
            
            if (dwBufSize > 0)
            {
                if ( !PlayM4_SetStreamOpenMode(lPort, STREAME_REALTIME) )  /* ����ʵʱ������ģʽ */
                {
                    break;
                }
                
                if ( !PlayM4_OpenStream(lPort, pBuffer, dwBufSize, 1024 * 1024) ) /* �����ӿ� */
                {
                    break;
                }
                
                if ( !PlayM4_Play(lPort, hWnd) ) /* ���ſ�ʼ */
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
        
        case NET_DVR_STREAMDATA:   /* �������� */
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

void CALLBACK CameraCtrl::ExceptionCallBack(DWORD dwType, LONG lUserID, LONG lHandle, void *pUser)
{
    printf("%s\n", __FUNCTION__ );
    
    switch (dwType) 
    {
        case EXCEPTION_RECONNECT:    /* Ԥ��ʱ���� */
        {
            printf("----------reconnect--------%d\n", time(NULL));
        }
        break;
        
        default:
        break;
    }
}

extern "C" void CALLBACK CCallExceptionCallBack(DWORD dwType, LONG lUserID, LONG lHandle, void *pUser )
{
    CameraCtrl* pFunc;
    return pFunc->ExceptionCallBack(dwType, lUserID, lHandle, pUser);
}

extern "C" void CALLBACK CCallRealDataCallBack_V30(LONG lRealHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, DWORD dwUser )
{
    CameraCtrl* pFunc;
    return pFunc->RealDataCallBack_V30( lRealHandle, dwDataType, pBuffer, dwBufSize, dwUser);
}

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
    //NET_DVR_SetExceptionCallBack_V30(0, NULL, g_ExceptionCallBack, NULL);
    
    NET_DVR_SetExceptionCallBack_V30(0, NULL, CCallExceptionCallBack, NULL);
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


