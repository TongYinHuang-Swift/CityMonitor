/**
 *=======================================================================================================================
 * Swift Technology 重庆思委夫特科技有限公司 城市视频监控系统软件
 * Name        : Camera.h
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
#ifndef _CAMERA_H_
#define _CAMERA_H_
#pragma once
#include "Global.h" 
#include "BasicObj.h" 
#include "HCNetSDK.h"

class CameraCtrl;
class VideoPlayer;


class Camera : public BasicObj
{
public:
    Camera(CameraCtrl* cameraCtrl, VideoPlayer* videoPlayer);
    virtual ~Camera();

private:
    CameraCtrl*     cameraCtrl;     // 摄像机的控制器
    VideoPlayer*    videoPlayer;    // 摄像机的播放器
    char*           ipAddr;         // 摄像机的IP地址

protected:
  
public:
    void    Init();                 // 摄像机初始化
    LONG    Login(char *pDVRIP, WORD wDVRPort, char *pUserName, char *pPassword, LPNET_DVR_DEVICEINFO_V30 DeviceInfo);    // 登录具有指定IP地址的摄像机
    void    Play();                 // 正常速度播放
    void    PausePlay();            // 暂停播放
    void    ResumePlay();           // 恢复正常速度播放
    void    StopPlay();             // 停止播放
    void    PlayFast();             // 按指定速度快速播放
    void    PlaySlow();             // 按指定速度慢速播放
    void    PlayOneFrame();         // 播放一帧
    void    PlayOneByOne();         // 每秒播放一帧，连续播放
    void    PlayFastBack();         // 按指定速度快速倒退
    void    PlaySlowBack();         // 按指定速度慢速倒退
    void    PlayOneFrameBack();     // 倒退一帧
    void    PlayOneByOneBack();     // 每秒倒退一帧，连续播放
    void    SetPlaySpeed(int speed);// 设置播放速度
};
#endif