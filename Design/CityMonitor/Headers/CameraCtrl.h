/**
 *=======================================================================================================================
 * Swift Technology ����˼ί���ؿƼ����޹�˾ ������Ƶ���ϵͳ���
 * Name        : CameraCtrl.h
 * Author      : ����
 * Version     : V1.0.0
 * Copyright   : �����������˼ί���ؿƼ����޹�˾������ӵ������Ȩ��������˼ί����������Ȩ��ɵ�����£��κ�������������
 *               ���öԱ��ļ����в��ֻ�ȫ���Ŀ��������򣬱���˾������׷����Ӧ�ķ������Ρ�
 * Description : ������ӿ�ģ��
 *=======================================================================================================================
 * Revision History:
 * �޸�ʱ��    �޸���  �޸�����
 * 2016-07-08  ����    �½�
 * 2016-07-xx  ����    ��ɰ汾V1.0.0
 *=======================================================================================================================
 */
#ifndef _CAMERACTRL_H_
#define _CAMERACTRL_H_
#pragma once
#include "Global.h" 
#include "BasicObj.h" 
#include "HCNetSDK.h"

#define CAMERA_PORT         (8000)
#define CAMERA_USER_NAME    ("admin")
#define CAMERA_PASSWORD     ("admin0123")

#ifdef __cplusplus
extern "C" {
#endif

void CALLBACK CCallRealDataCallBack_V30(LONG lRealHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, DWORD dwUser);

#ifdef __cplusplus
    }
#endif

class CameraCtrl : public BasicObj
{
public:
    CameraCtrl(void);
    virtual         ~CameraCtrl(void);

private:
    LONG                        lUserID;
    LPNET_DVR_DEVICEINFO_V30    deviceInfo;
    
protected:
  
public:
    void            Init(void);                 /* �������������ʼ�� */
    void            SetLoginConfig(LPNET_DVR_DEVICEINFO_V30 *pDeviceInfo);
    LONG            Login(char *ipAddr);		/* ��¼����ָ��IP��ַ������� */
    LONG            GetUsrID(void);
    void            CALLBACK ExceptionCallBack(DWORD dwType, LONG lUserID, LONG lHandle, void *pUser);
    void            Exit(void);
};
#endif

