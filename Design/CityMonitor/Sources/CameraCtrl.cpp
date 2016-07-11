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
#include "CameraCtrl.h"

#ifdef WIN32
#else
#endif

CameraCtrl::CameraCtrl(void)
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
    
    /* ��ʼ������ͷ��½���� */
    memset( &(this->deviceInfo), 0, sizeof(LPNET_DVR_DEVICEINFO_V30) );
}

void CameraCtrl::SetLoginConfig(LPNET_DVR_DEVICEINFO_V30 *pDeviceInfo)
{
    if (pDeviceInfo)
    {
        memcpy( &this->deviceInfo, pDeviceInfo, sizeof(LPNET_DVR_DEVICEINFO_V30) );
    }
}


#ifdef __cplusplus
extern "C" {
#endif

void CALLBACK CCallExceptionCallBack(DWORD dwType, LONG lUserID, LONG lHandle, void *pUser)
{
    //CameraCtrl* pFunc = new CameraCtrl();
    //pFunc->ExceptionCallBack(dwType, lUserID, lHandle, pUser);
    //delete pFunc;
    ((CameraCtrl *)pUser)->ExceptionCallBack(dwType, lUserID, lHandle, pUser);
}

#ifdef __cplusplus
    }
#endif /* ENDIF __cplusplus */

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

/***
 * desc:            ��¼����ָ��IP��ַ�������
 * pDVRIP:          ����ͷIP
 * wDVRPort:        ����ͷ�˿ڣ�Ĭ��8000
 * pUserName:       ��½�û���Ĭ��admin
 * pPassword:       ��½���룬Ĭ��admin
 * pDeviceInfo:     �豸��Ϣ
 * retc:            UserID--Success, -1--Error
 */
LONG CameraCtrl::Login(char *ipAddr)
{
    //LPNET_DVR_DEVICEINFO_V30 deviceInfo;

    memset( &deviceInfo, 0, sizeof(LPNET_DVR_DEVICEINFO_V30) );

    lUserID = NET_DVR_Login_V30(ipAddr, CAMERA_PORT, CAMERA_USER_NAME, CAMERA_PASSWORD, deviceInfo);
    
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
    NET_DVR_SetExceptionCallBack_V30(0, NULL, CCallExceptionCallBack, this);
    return lUserID;
}

/***
 * desc:            ��ȡ����ͷ�û�ID
 * para:            None
 * retc:            UserID
 */
LONG CameraCtrl::GetUsrID(void)
{
    return this->lUserID;
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


