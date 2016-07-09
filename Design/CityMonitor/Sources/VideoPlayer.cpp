/**
 *=======================================================================================================================
 * Swift Technology ����˼ί���ؿƼ����޹�˾ ������Ƶ���ϵͳ���
 * Name        : VideoPlayer.cpp
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
#include "VideoPlayer.h"
#include "Debugger.h" 

#ifdef WIN32
#else
#endif

VideoPlayer::VideoPlayer()
{
    PRINT(DEBUG_LEVEL_9, "VideoPlayer", __FUNCTION__, __LINE__);
    speed = 0;
}
VideoPlayer::~VideoPlayer()
{
    PRINT(DEBUG_LEVEL_9, "VideoPlayer", __FUNCTION__, __LINE__);
}
// ��������ʼ��
void VideoPlayer::Init()
{
    PRINT(DEBUG_LEVEL_9, "VideoPlayer", __FUNCTION__, __LINE__);
}
// �����ٶȲ���
void VideoPlayer::Play()
{
    PRINT(DEBUG_LEVEL_9, "VideoPlayer", __FUNCTION__, __LINE__);
}                   
// ��ͣ����
void VideoPlayer::Pause()
{
    PRINT(DEBUG_LEVEL_9, "VideoPlayer", __FUNCTION__, __LINE__);
}
// �ָ������ٶȲ���
void VideoPlayer::Resume()
{
    PRINT(DEBUG_LEVEL_9, "VideoPlayer", __FUNCTION__, __LINE__);
}   






// ֹͣ����
//void VideoPlayer::Stop()
//{
//    PRINT(DEBUG_LEVEL_9, "VideoPlayer", __FUNCTION__, __LINE__);
//}                     
// ��ָ���ٶȿ��ٲ���
void VideoPlayer::PlayFast()
{
    PRINT(DEBUG_LEVEL_9, "VideoPlayer", __FUNCTION__, __LINE__);
}               
// ��ָ���ٶ����ٲ���
void VideoPlayer::PlaySlow()
{
    PRINT(DEBUG_LEVEL_9, "VideoPlayer", __FUNCTION__, __LINE__);
}               
// ����һ֡
void VideoPlayer::PlayOneFrame()
{
    PRINT(DEBUG_LEVEL_9, "VideoPlayer", __FUNCTION__, __LINE__);
}           
// ÿ�벥��һ֡����������
void VideoPlayer::PlayOneByOne()
{
    PRINT(DEBUG_LEVEL_9, "VideoPlayer", __FUNCTION__, __LINE__);
}           
// ��ָ���ٶȿ��ٵ���
void VideoPlayer::PlayFastBack()
{
    PRINT(DEBUG_LEVEL_9, "VideoPlayer", __FUNCTION__, __LINE__);
}           
// ��ָ���ٶ����ٵ���
void VideoPlayer::PlaySlowBack()
{
    PRINT(DEBUG_LEVEL_9, "VideoPlayer", __FUNCTION__, __LINE__);
}           
// ����һ֡
void VideoPlayer::PlayOneFrameBack()
{
    PRINT(DEBUG_LEVEL_9, "VideoPlayer", __FUNCTION__, __LINE__);
}
// ÿ�뵹��һ֡����������
void VideoPlayer::PlayOneByOneBack()
{
    PRINT(DEBUG_LEVEL_9, "VideoPlayer", __FUNCTION__, __LINE__);
}
// ���ò����ٶ�
void VideoPlayer::SetPlaySpeed(int speed)
{
    PRINT(DEBUG_LEVEL_2, "VideoPlayer", __FUNCTION__, __LINE__);
    this->speed = speed;
}       

