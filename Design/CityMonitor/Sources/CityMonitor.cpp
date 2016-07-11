/**
 *=======================================================================================================================
 * Swift Technology 重庆思委夫特科技有限公司 城市视频监控系统软件
 * Name        : CityMonitor.cpp
 * Author      : 董超
 * Version     : V1.0.0
 * Copyright   : 本软件由重庆思委夫特科技有限公司开发并拥有所有权利，在无思委夫特书面授权许可的情况下，任何其他团体或个人
 *               不得对本文件进行部分或全部的拷贝，否则，本公司将依法追究相应的法律责任。
 * Description : 城市视频监控系统执行程序入口
 *=======================================================================================================================
 * Revision History:
 * 修改时间    修改人  修改内容
 * 2016-06-10  董超    新建
 *=======================================================================================================================
 */
#include "VideoManager.h"
#include <time.h>
#ifdef WIN32
#include <windows.h>
#endif
#include "VideoRecorder.h"
#include "Camera.h"
#include "CameraCtrl.h"
#include "MP4Player.h"
#include "Buffer.h"
#include "Debugger.h"
#include <cstring>
#ifndef WIN32
#include <unistd.h>
#endif

#define CAMERA_IPADDR ("192.168.1.65")

int main() 
{
#if 0
    //VideoManager* videoManager = new VideoManager();

    // 创建摄像机控制器
    CameraCtrl* cameraCtrl = new CameraCtrl();
    // 创建摄像机视频播放器
    MP4Player* mp4Player = new MP4Player();
    // 创建摄像机
    Camera* camera = new Camera(CAMERA_IPADDR, cameraCtrl, mp4Player);
    // 创建摄像机管理模块
    VideoManager* videoManager = new VideoManager(camera);
    // 运行整个系统
    videoManager->Run();

    // 删除所有对象，释放内存
    delete cameraCtrl;
    delete mp4Player;
    delete camera;
    delete videoManager;
#endif

#define TEST_REAL_PLAY_0

#ifdef TEST_REAL_PLAY_0
    /* 创建摄像机控制器 */
    CameraCtrl* cameraCtrl = new CameraCtrl();
    cameraCtrl->Init();
    LONG lUserID = cameraCtrl->Login( CAMERA_IPADDR );

    /* 创建摄像机视频播放器 */
    MP4Player *mp4Player = new MP4Player();
    mp4Player->SetCameraCtrl(cameraCtrl);
    mp4Player->Init();
#ifdef WIN32
    mp4Player->SetConsoleWindow(GetConsoleWindow());
#endif
    /* 设置存放实时数据缓存 */
    //mp4Player->SetRealVideoBuffer(cameraCtrlBuffer);
    if ( lUserID != -1 )
    {
        mp4Player->RealPlayInit(lUserID);
    }

    // 创建摄像机
    Camera* camera = new Camera(CAMERA_IPADDR, cameraCtrl, mp4Player);
    
    // 创建摄像机管理模块
    VideoManager* videoManager = new VideoManager(camera);
    
    // 运行整个系统
    videoManager->Run();

    // 删除所有对象，释放内存
    delete videoManager;
    delete camera;
    delete mp4Player;
    delete cameraCtrl;
#endif

    while (1)
    {
        #ifdef WIN32
            Sleep(1000);
        #else
            sleep(1000);
        #endif
    }

    return 0;
}
