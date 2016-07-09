/**
 *=======================================================================================================================
 * Swift Technology 重庆思委夫特科技有限公司 城市视频监控系统软件
 * Name        : VideoPlayer.h
 * Author      : 董超
 * Version     : V1.0.0
 * Copyright   : 本软件由重庆思委夫特科技有限公司开发并拥有所有权利，在无思委夫特书面授权许可的情况下，任何其他团体或个人
 *               不得对本文件进行部分或全部的拷贝，否则，本公司将依法追究相应的法律责任。
 * Description : 摄像机视频流播放模块
 *=======================================================================================================================
 * Revision History:
 * 修改时间    修改人  修改内容
 * 2016-07-08  董超    新建
 * 2016-07-xx  董超    完成版本V1.0.0
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
    int     speed;                      // 播放速度

protected:
  
public:
    virtual void    Init();                     // 播放器初始化
    virtual void    Play();                     // 正常速度播放
    virtual void    Pause();                    // 暂停播放
    virtual void    Resume();                   // 恢复正常速度播放
    virtual void    Stop();                     // 停止播放
    virtual void    PlayFast();                 // 按指定速度快速播放
    virtual void    PlaySlow();                 // 按指定速度慢速播放
    virtual void    PlayOneFrame();             // 播放一帧
    virtual void    PlayOneByOne();             // 每秒播放一帧，连续播放
    virtual void    PlayFastBack();             // 按指定速度快速倒退
    virtual void    PlaySlowBack();             // 按指定速度慢速倒退
    virtual void    PlayOneFrameBack();         // 倒退一帧
    virtual void    PlayOneByOneBack();         // 每秒倒退一帧，连续播放
    void            SetPlaySpeed(int speed);    // 设置播放速度
};
#endif
