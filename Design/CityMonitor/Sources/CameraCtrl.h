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


class CameraCtrl : public BasicObj
{
public:
    CameraCtrl(void);
    virtual ~CameraCtrl(void);

private:
    LONG lUserID;

protected:
  
public:
    void    Init(void);                 // �������������ʼ��
    LONG    Login(char *pDVRIP, WORD wDVRPort, char *pUserName, char *pPassword, LPNET_DVR_DEVICEINFO_V30 *pDeviceInfo);    // ��¼����ָ��IP��ַ�������
    LONG    GetUsrID(void);
    void    Exit(void);
};
#endif
