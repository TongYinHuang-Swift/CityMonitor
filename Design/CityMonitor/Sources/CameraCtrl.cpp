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
 * desc:            ��¼����ָ��IP��ַ�������
 * pDVRIP:          ����ͷIP
 * wDVRPort:        ����ͷ�˿ڣ�Ĭ��8000
 * pUserName:       ��½�û���Ĭ��admin
 * pPassword:       ��½���룬Ĭ��admin
 * pDeviceInfo:     �豸��Ϣ
 * retc:            UserID--Success, -1--Error
 */
LONG CameraCtrl::Login(char *pDVRIP, WORD wDVRPort, char *pUserName, char *pPassword, LPNET_DVR_DEVICEINFO_V30 *pDeviceInfo)
{
    lUserID = NET_DVR_Login_V30( pDVRIP, wDVRPort, pUserName, pPassword, pDeviceInfo );
    
    if ( lUserID < 0)
    {
        PRINT("Login error, %d\n", NET_DVR_GetLastError());
        NET_DVR_Cleanup();
        return -1;
    }
    else
    {
        PRINT("Login success!\n");
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


