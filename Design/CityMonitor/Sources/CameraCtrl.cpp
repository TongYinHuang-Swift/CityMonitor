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
 * desc:            摄像头视频流回调函数
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
        case NET_DVR_SYSHEAD:               /* 系统头 */
        {
            if ( !PlayM4_GetPort(&lPort) )  /* 获取播放库未使用的通道号 */
            {
                break;
            }
            
            /* 第一次回调的是系统头，将获取的播放库port号赋值给全局port，下次回调数据时即使用此port号播放 */
            
            if (dwBufSize > 0)
            {
                if ( !PlayM4_SetStreamOpenMode(lPort, STREAME_REALTIME) )  /* 设置实时流播放模式 */
                {
                    break;
                }
                
                if ( !PlayM4_OpenStream(lPort, pBuffer, dwBufSize, 1024 * 1024) ) /* 打开流接口 */
                {
                    break;
                }
                
                if ( !PlayM4_Play(lPort, hWnd) ) /* 播放开始 */
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
        
        case NET_DVR_STREAMDATA:   /* 码流数据 */
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
        case EXCEPTION_RECONNECT:    /* 预览时重连 */
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
 * desc:            登录具有指定IP地址的摄像机
 * pDVRIP:          摄像头IP
 * wDVRPort:        摄像头端口，默认8000
 * pUserName:       登陆用户，默认admin
 * pPassword:       登陆密码，默认admin
 * pDeviceInfo:     设备信息
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
    
    /* 设置异常消息回调函数 */
    //NET_DVR_SetExceptionCallBack_V30(0, NULL, g_ExceptionCallBack, NULL);
    
    NET_DVR_SetExceptionCallBack_V30(0, NULL, CCallExceptionCallBack, NULL);
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

void CameraCtrl::SetPlayPort( LONG SetPort )
{
    lPort = SetPort;
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


