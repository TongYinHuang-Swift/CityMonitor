/**
 *=======================================================================================================================
 * Swift Technology ����˼ί���ؿƼ����޹�˾ ������Ƶ���ϵͳ���
 * Name        : MP4Player.h
 * Author      : ����
 * Version     : V1.0.0
 * Copyright   : �����������˼ί���ؿƼ����޹�˾������ӵ������Ȩ��������˼ί����������Ȩ��ɵ�����£��κ�������������
 *               ���öԱ��ļ����в��ֻ�ȫ���Ŀ��������򣬱���˾������׷����Ӧ�ķ������Ρ�
 * Description : �����MP4��ʽ��Ƶ������ģ��
 *=======================================================================================================================
 * Revision History:
 * �޸�ʱ��    �޸���  �޸�����
 * 2016-07-08  ����    �½�
 * 2016-07-xx  ����    ��ɰ汾V1.0.0
 *=======================================================================================================================
 */
#ifndef _MP4PLAYER_H_
#define _MP4PLAYER_H_
#pragma once
#include "Global.h" 
#include "VideoPlayer.h" 
#include "plaympeg4.h"
#include "HCNetSDK.h"

typedef enum 
{
    SW_HIK_PLAY_START              = 0,
    SW_HIK_PLAY_PAUSE              = 1,
    SW_HIK_PLAY_STOP               = 2,
    SW_HIK_PLAY_RESUME             = 3,
    SW_HIK_PLAY_SLOW_FWD           = 4,
    SW_HIK_PLAY_FAST_FWD           = 5,
    SW_HIK_PLAY_ONE_BY_ONE         = 6,
    SW_HIK_PLAY_SLOW_BACK          = 7,
    SW_HIK_PLAY_FAST_BACK          = 8,
    SW_HIK_PLAY_ONE_BY_ONE_BACK    = 9,
} SW_HIK_PLAY_CTRL_T;

typedef enum
{
    PLAY_MODE_START = 0,
    PLAY_MODE_PAUSE = 1,
} PLAY_MODE_T;

class MP4Player : public VideoPlayer
{
public:
    MP4Player();
    virtual ~MP4Player();

private:
    LONG lPort;
    LONG lUserID;
    LONG lRealPlayHandle;
protected:
  
public:
    void    Init(void);                     // ��������ʼ��
    void    RealPlayStart(void);       //��ʼʵʱԤ��
    void    RealPlayStop(void);         //ֹͣԤ��
    int     PlayLocalFile( LPSTR sFileName ); //���ű����ļ�
    void    PlayLocalFileExit(void); //�˳����ű����ļ�
    void    Play(void);                     // �����ٶȲ���
    void    Pause(void);                    // ��ͣ����
    void    Resume(void);                   // �ָ������ٶȲ���
    void    Stop(void);                     // ֹͣ����
    void    PlayFast(void);                 // ��ָ���ٶȿ��ٲ���
    void    PlaySlow(void);                 // ��ָ���ٶ����ٲ���
    void    PlayOneFrame(void);             // ����һ֡
    void    PlayOneByOne(void);             // ÿ�벥��һ֡����������
    void    PlayFastBack(void);             // ��ָ���ٶȿ��ٵ���
    void    PlaySlowBack(void);             // ��ָ���ٶ����ٵ���
    void    PlayOneFrameBack(void);         // ����һ֡
    void    PlayOneByOneBack(void);         // ÿ�뵹��һ֡����������
};
#endif
