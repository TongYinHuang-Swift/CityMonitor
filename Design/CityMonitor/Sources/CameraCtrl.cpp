/**
 *=======================================================================================================================
 * Swift Technology 重庆思委夫特科技有限公司 城市视频监控系统软件
 * Name        : CameraCtrl.cpp
 * Author      : 董超
 * Version     : V1.0.0
 * Copyright   : 本软件由重庆思委夫特科技有限公司开发并拥有所有权利，在无思委夫特书面授权许可的情况下，任何其他团体或个人
 *               不得对本文件进行部分或全部的拷贝，否则，本公司将依法追究相应的法律责任。
 * Description : 摄像机控制模块
 *=======================================================================================================================
 * Revision History:
 * 修改时间    修改人  修改内容
 * 2016-07-08  董超    新建
 * 2016-07-xx  董超    完成版本V1.0.0
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
 * desc:            摄像头初始化
 * para:            None
 * retc:            None
 */
void CameraCtrl::Init(void)
{
    /* 初始化海康摄像头SDK */
    NET_DVR_Init();
}

/***
 * desc:            登录具有指定IP地址的摄像机
 * pDVRIP:          摄像头IP
 * wDVRPort:        摄像头端口，默认8000
 * pUserName:       登陆用户，默认admin
 * pPassword:       登陆密码，默认admin
 * pDeviceInfo:     设备信息
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
    
    /* 设置异常消息回调函数 */
    NET_DVR_SetExceptionCallBack_V30(0, NULL, g_ExceptionCallBack, NULL);
    return lUserID;
}

/***
 * desc:            获取摄像头用户ID
 * para:            None
 * retc:            UserID
 */
LONG CameraCtrl::GetUsrID(void)
{
    return lUserID;
}

/***
 * desc:            摄像头登出
 * para:            None
 * retc:            None
 */
void CameraCtrl::Exit(void)
{
    /* 注销用户 */
    NET_DVR_Logout_V30(lUserID);
    NET_DVR_Cleanup();
}


