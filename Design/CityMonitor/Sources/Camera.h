/**
 *=======================================================================================================================
 * Swift Technology ����˼ί���ؿƼ����޹�˾ ������Ƶ���ϵͳ����
 * Name        : Camera.h
 * Author      : ����
 * Version     : V1.0.0
 * Copyright   : ������������˼ί���ؿƼ����޹�˾������ӵ������Ȩ��������˼ί����������Ȩ���ɵ�����£��κ�������������
 *               ���öԱ��ļ����в��ֻ�ȫ���Ŀ��������򣬱���˾������׷����Ӧ�ķ������Ρ�
 * Description : ������ӿ�ģ��
 *=======================================================================================================================
 * Revision History:
 * �޸�ʱ��    �޸���  �޸�����
 * 2016-07-08  ����    �½�
 * 2016-07-xx  ����    ��ɰ汾V1.0.0
 *=======================================================================================================================
 */
#ifndef _CAMERA_H_
#define _CAMERA_H_
#pragma once
#include "Global.h" 
#include "BasicObj.h" 

class CameraCtrl;
class VideoPlayer;


class Camera : public BasicObj
{
public:
    Camera(CameraCtrl* cameraCtrl, VideoPlayer* videoPlayer);
    virtual ~Camera();

private:
    CameraCtrl*     cameraCtrl;     // ������Ŀ�����
    VideoPlayer*    videoPlayer;    // ������Ĳ�����
    char*           ipAddr;         // �������IP��ַ

protected:
  
public:
    void    Init();                 // �������ʼ��
    void    Login(char* ipAddr);    // ��¼����ָ��IP��ַ�������
    void    Play();                 // �����ٶȲ���
    void    PausePlay();            // ��ͣ����
    void    ResumePlay();           // �ָ������ٶȲ���
    void    StopPlay();             // ֹͣ����
    void    PlayFast();             // ��ָ���ٶȿ��ٲ���
    void    PlaySlow();             // ��ָ���ٶ����ٲ���
    void    PlayOneFrame();         // ����һ֡
    void    PlayOneByOne();         // ÿ�벥��һ֡����������
    void    PlayFastBack();         // ��ָ���ٶȿ��ٵ���
    void    PlaySlowBack();         // ��ָ���ٶ����ٵ���
    void    PlayOneFrameBack();     // ����һ֡
    void    PlayOneByOneBack();     // ÿ�뵹��һ֡����������
    void    SetPlaySpeed(int speed);// ���ò����ٶ�
};
#endif