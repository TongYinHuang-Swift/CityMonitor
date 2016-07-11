/**
 *=======================================================================================================================
 * Swift Technology ����˼ί���ؿƼ����޹�˾ ������Ƶ���ϵͳ���
 * Name        : CityMonitor.cpp
 * Author      : ����
 * Version     : V1.0.0
 * Copyright   : �����������˼ί���ؿƼ����޹�˾������ӵ������Ȩ��������˼ί����������Ȩ��ɵ�����£��κ�������������
 *               ���öԱ��ļ����в��ֻ�ȫ���Ŀ��������򣬱���˾������׷����Ӧ�ķ������Ρ�
 * Description : ������Ƶ���ϵͳִ�г������
 *=======================================================================================================================
 * Revision History:
 * �޸�ʱ��    �޸���  �޸�����
 * 2016-06-10  ����    �½�
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

    // ���������������
    CameraCtrl* cameraCtrl = new CameraCtrl();
    // �����������Ƶ������
    MP4Player* mp4Player = new MP4Player();
    // ���������
    Camera* camera = new Camera(CAMERA_IPADDR, cameraCtrl, mp4Player);
    // �������������ģ��
    VideoManager* videoManager = new VideoManager(camera);
    // ��������ϵͳ
    videoManager->Run();

    // ɾ�����ж����ͷ��ڴ�
    delete cameraCtrl;
    delete mp4Player;
    delete camera;
    delete videoManager;
#endif

#define TEST_REAL_PLAY_0

#ifdef TEST_REAL_PLAY_0
    /* ��������������� */
    CameraCtrl* cameraCtrl = new CameraCtrl();
    cameraCtrl->Init();
    LONG lUserID = cameraCtrl->Login( CAMERA_IPADDR );

    /* �����������Ƶ������ */
    MP4Player *mp4Player = new MP4Player();
    mp4Player->SetCameraCtrl(cameraCtrl);
    mp4Player->Init();
#ifdef WIN32
    mp4Player->SetConsoleWindow(GetConsoleWindow());
#endif
    /* ���ô��ʵʱ���ݻ��� */
    //mp4Player->SetRealVideoBuffer(cameraCtrlBuffer);
    if ( lUserID != -1 )
    {
        mp4Player->RealPlayInit(lUserID);
    }

    // ���������
    Camera* camera = new Camera(CAMERA_IPADDR, cameraCtrl, mp4Player);
    
    // �������������ģ��
    VideoManager* videoManager = new VideoManager(camera);
    
    // ��������ϵͳ
    videoManager->Run();

    // ɾ�����ж����ͷ��ڴ�
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
