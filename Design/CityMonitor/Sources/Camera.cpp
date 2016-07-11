/**
 *=======================================================================================================================
 * Swift Technology 重庆思委夫特科技有限公司 城市视频监控系统软件
 * Name        : Camera.cpp
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
#include "Camera.h"
#include "CameraCtrl.h" 
#include "VideoPlayer.h" 
#include "Debugger.h" 

#ifdef WIN32
#else
#endif

Camera::Camera(char *ipAddr, CameraCtrl* cameraCtrl, VideoPlayer* videoPlayer)
{
	strcpy(this->ipAddr, ipAddr);
    this->cameraCtrl  = cameraCtrl;
    this->videoPlayer = videoPlayer;
}
Camera::~Camera()
{
}
void Camera::Init()
{
    cameraCtrl->Init();
    videoPlayer->Init();
}
LONG Camera::Login()
{
    return cameraCtrl->Login(ipAddr);
}
void Camera::SetPlaySpeed(int speed)
{
    videoPlayer->SetPlaySpeed(speed);
}
// 正常速度播放
void Camera::Play()
{
    PRINT(DEBUG_LEVEL_9, "Camera", __FUNCTION__, __LINE__);
    videoPlayer->Play();
}                   
// 暂停播放
void Camera::PausePlay()
{
    PRINT(DEBUG_LEVEL_1, "Camera", __FUNCTION__, __LINE__);
    videoPlayer->Pause();
}
// 恢复正常速度播放
void Camera::ResumePlay()
{
    PRINT(DEBUG_LEVEL_1, "Camera", __FUNCTION__, __LINE__);
    videoPlayer->Resume();
}               
// 停止播放
void Camera::StopPlay()
{
    PRINT(DEBUG_LEVEL_1, "Camera", __FUNCTION__, __LINE__);
    videoPlayer->Stop();
}                       
// 按指定速度快速播放
void Camera::PlayFast()
{
    PRINT(DEBUG_LEVEL_4, "Camera", __FUNCTION__, __LINE__);
    videoPlayer->PlayFast();
}               
// 按指定速度慢速播放
void Camera::PlaySlow()
{
    PRINT(DEBUG_LEVEL_3, "Camera", __FUNCTION__, __LINE__);
    videoPlayer->PlaySlow();
}               
// 播放一帧
void Camera::PlayOneFrame()
{
    PRINT(DEBUG_LEVEL_1, "Camera", __FUNCTION__, __LINE__);
    videoPlayer->PlayOneFrame();
}           
// 每秒播放一帧，连续播放
void Camera::PlayOneByOne()
{
    PRINT(DEBUG_LEVEL_1, "Camera", __FUNCTION__, __LINE__);
    videoPlayer->PlayOneByOne();
}           
// 按指定速度快速倒退
void Camera::PlayFastBack()
{
    PRINT(DEBUG_LEVEL_4, "Camera", __FUNCTION__, __LINE__);
    videoPlayer->PlayFastBack();
}           
// 按指定速度慢速倒退
void Camera::PlaySlowBack()
{
    PRINT(DEBUG_LEVEL_3, "Camera", __FUNCTION__, __LINE__);
    videoPlayer->PlaySlowBack();
}           
// 倒退一帧
void Camera::PlayOneFrameBack()
{
    PRINT(DEBUG_LEVEL_1, "Camera", __FUNCTION__, __LINE__);
    videoPlayer->PlayOneFrameBack();
}       
// 每秒倒退一帧，连续播放
void Camera::PlayOneByOneBack()
{
    PRINT(DEBUG_LEVEL_1, "Camera", __FUNCTION__, __LINE__);
    videoPlayer->PlayOneByOneBack();
}

