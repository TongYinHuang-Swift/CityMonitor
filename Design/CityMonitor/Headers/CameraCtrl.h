/**
 *=======================================================================================================================
 * Swift Technology 重庆思委夫特科技有限公司 城市视频监控系统软件
 * Name        : CameraCtrl.h
 * Author      : 董超
 * Version     : V1.0.0
 * Copyright   : 本软件由重庆思委夫特科技有限公司开发并拥有所有权利，在无思委夫特书面授权许可的情况下，任何其他团体或个人
 *               不得对本文件进行部分或全部的拷贝，否则，本公司将依法追究相应的法律责任。
 * Description : 摄像机接口模块
 *=======================================================================================================================
 * Revision History:
 * 修改时间    修改人  修改内容
 * 2016-07-08  董超    新建
 * 2016-07-xx  董超    完成版本V1.0.0
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
    void            Init(void);                 /* 摄像机控制器初始化 */
    void            SetLoginConfig(LPNET_DVR_DEVICEINFO_V30 *pDeviceInfo);
    LONG            Login(char *ipAddr);		/* 登录具有指定IP地址的摄像机 */
    LONG            GetUsrID(void);
    void            CALLBACK ExceptionCallBack(DWORD dwType, LONG lUserID, LONG lHandle, void *pUser);
    void            Exit(void);
};
#endif

