/**
 *=======================================================================================================================
 * Swift Technology ����˼ί���ؿƼ����޹�˾ ������Ƶ���ϵͳ���
 * Name        : VideoPlayer.h
 * Author      : ����
 * Version     : V1.0.0
 * Copyright   : �����������˼ί���ؿƼ����޹�˾������ӵ������Ȩ��������˼ί����������Ȩ��ɵ�����£��κ�������������
 *               ���öԱ��ļ����в��ֻ�ȫ���Ŀ��������򣬱���˾������׷����Ӧ�ķ������Ρ�
 * Description : �������Ƶ������ģ��
 *=======================================================================================================================
 * Revision History:
 * �޸�ʱ��    �޸���  �޸�����
 * 2016-07-08  ����    �½�
 * 2016-07-xx  ����    ��ɰ汾V1.0.0
 *=======================================================================================================================
 */
#ifndef _VIDEOPLAYER_H_
#define _VIDEOPLAYER_H_
#pragma once
#include "Global.h" 
#include "BasicObj.h" 


class VideoPlayer : public BasicObj
{
public:
    VideoPlayer();
    virtual ~VideoPlayer();

private:
    int     speed;                      // �����ٶ�

protected:
  
public:
    virtual void    Init();                     // ��������ʼ��
    virtual void    Play();                     // �����ٶȲ���
    virtual void    Pause();                    // ��ͣ����
    virtual void    Resume();                   // �ָ������ٶȲ���
    virtual void    Stop();                     // ֹͣ����
    virtual void    PlayFast();                 // ��ָ���ٶȿ��ٲ���
    virtual void    PlaySlow();                 // ��ָ���ٶ����ٲ���
    virtual void    PlayOneFrame();             // ����һ֡
    virtual void    PlayOneByOne();             // ÿ�벥��һ֡����������
    virtual void    PlayFastBack();             // ��ָ���ٶȿ��ٵ���
    virtual void    PlaySlowBack();             // ��ָ���ٶ����ٵ���
    virtual void    PlayOneFrameBack();         // ����һ֡
    virtual void    PlayOneByOneBack();         // ÿ�뵹��һ֡����������
    void            SetPlaySpeed(int speed);    // ���ò����ٶ�
};
#endif
