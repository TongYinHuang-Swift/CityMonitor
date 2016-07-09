/**
 *=======================================================================================================================
 * Swift Technology 重庆思委夫特科技有限公司 城市视频监控系统软件
 * Name        : VideoPlayer.cpp
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
// 播放器初始化
void VideoPlayer::Init()
{
    PRINT(DEBUG_LEVEL_9, "VideoPlayer", __FUNCTION__, __LINE__);
}
// 正常速度播放
void VideoPlayer::Play()
{
    PRINT(DEBUG_LEVEL_9, "VideoPlayer", __FUNCTION__, __LINE__);
}                   
// 暂停播放
void VideoPlayer::Pause()
{
    PRINT(DEBUG_LEVEL_9, "VideoPlayer", __FUNCTION__, __LINE__);
}
// 恢复正常速度播放
void VideoPlayer::Resume()
{
    PRINT(DEBUG_LEVEL_9, "VideoPlayer", __FUNCTION__, __LINE__);
}   






// 停止播放
//void VideoPlayer::Stop()
//{
//    PRINT(DEBUG_LEVEL_9, "VideoPlayer", __FUNCTION__, __LINE__);
//}                     
// 按指定速度快速播放
void VideoPlayer::PlayFast()
{
    PRINT(DEBUG_LEVEL_9, "VideoPlayer", __FUNCTION__, __LINE__);
}               
// 按指定速度慢速播放
void VideoPlayer::PlaySlow()
{
    PRINT(DEBUG_LEVEL_9, "VideoPlayer", __FUNCTION__, __LINE__);
}               
// 播放一帧
void VideoPlayer::PlayOneFrame()
{
    PRINT(DEBUG_LEVEL_9, "VideoPlayer", __FUNCTION__, __LINE__);
}           
// 每秒播放一帧，连续播放
void VideoPlayer::PlayOneByOne()
{
    PRINT(DEBUG_LEVEL_9, "VideoPlayer", __FUNCTION__, __LINE__);
}           
// 按指定速度快速倒退
void VideoPlayer::PlayFastBack()
{
    PRINT(DEBUG_LEVEL_9, "VideoPlayer", __FUNCTION__, __LINE__);
}           
// 按指定速度慢速倒退
void VideoPlayer::PlaySlowBack()
{
    PRINT(DEBUG_LEVEL_9, "VideoPlayer", __FUNCTION__, __LINE__);
}           
// 倒退一帧
void VideoPlayer::PlayOneFrameBack()
{
    PRINT(DEBUG_LEVEL_9, "VideoPlayer", __FUNCTION__, __LINE__);
}
// 每秒倒退一帧，连续播放
void VideoPlayer::PlayOneByOneBack()
{
    PRINT(DEBUG_LEVEL_9, "VideoPlayer", __FUNCTION__, __LINE__);
}
// 设置播放速度
void VideoPlayer::SetPlaySpeed(int speed)
{
    PRINT(DEBUG_LEVEL_2, "VideoPlayer", __FUNCTION__, __LINE__);
    this->speed = speed;
}       

