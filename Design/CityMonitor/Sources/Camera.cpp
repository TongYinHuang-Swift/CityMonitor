/**
 *=======================================================================================================================
 * Swift Technology ����˼ί���ؿƼ����޹�˾ ������Ƶ���ϵͳ���
 * Name        : Camera.cpp
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
// �����ٶȲ���
void Camera::Play()
{
    PRINT(DEBUG_LEVEL_9, "Camera", __FUNCTION__, __LINE__);
    videoPlayer->Play();
}                   
// ��ͣ����
void Camera::PausePlay()
{
    PRINT(DEBUG_LEVEL_1, "Camera", __FUNCTION__, __LINE__);
    videoPlayer->Pause();
}
// �ָ������ٶȲ���
void Camera::ResumePlay()
{
    PRINT(DEBUG_LEVEL_1, "Camera", __FUNCTION__, __LINE__);
    videoPlayer->Resume();
}               
// ֹͣ����
void Camera::StopPlay()
{
    PRINT(DEBUG_LEVEL_1, "Camera", __FUNCTION__, __LINE__);
    videoPlayer->Stop();
}                       
// ��ָ���ٶȿ��ٲ���
void Camera::PlayFast()
{
    PRINT(DEBUG_LEVEL_4, "Camera", __FUNCTION__, __LINE__);
    videoPlayer->PlayFast();
}               
// ��ָ���ٶ����ٲ���
void Camera::PlaySlow()
{
    PRINT(DEBUG_LEVEL_3, "Camera", __FUNCTION__, __LINE__);
    videoPlayer->PlaySlow();
}               
// ����һ֡
void Camera::PlayOneFrame()
{
    PRINT(DEBUG_LEVEL_1, "Camera", __FUNCTION__, __LINE__);
    videoPlayer->PlayOneFrame();
}           
// ÿ�벥��һ֡����������
void Camera::PlayOneByOne()
{
    PRINT(DEBUG_LEVEL_1, "Camera", __FUNCTION__, __LINE__);
    videoPlayer->PlayOneByOne();
}           
// ��ָ���ٶȿ��ٵ���
void Camera::PlayFastBack()
{
    PRINT(DEBUG_LEVEL_4, "Camera", __FUNCTION__, __LINE__);
    videoPlayer->PlayFastBack();
}           
// ��ָ���ٶ����ٵ���
void Camera::PlaySlowBack()
{
    PRINT(DEBUG_LEVEL_3, "Camera", __FUNCTION__, __LINE__);
    videoPlayer->PlaySlowBack();
}           
// ����һ֡
void Camera::PlayOneFrameBack()
{
    PRINT(DEBUG_LEVEL_1, "Camera", __FUNCTION__, __LINE__);
    videoPlayer->PlayOneFrameBack();
}       
// ÿ�뵹��һ֡����������
void Camera::PlayOneByOneBack()
{
    PRINT(DEBUG_LEVEL_1, "Camera", __FUNCTION__, __LINE__);
    videoPlayer->PlayOneByOneBack();
}

