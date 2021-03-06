/**
 *=======================================================================================================================
 * Swift Technology 重庆思委夫特科技有限公司 城市视频监控系统软件
 * Name        : MP4Player.h
 * Author      : 董超
 * Version     : V1.0.0
 * Copyright   : 本软件由重庆思委夫特科技有限公司开发并拥有所有权利，在无思委夫特书面授权许可的情况下，任何其他团体或个人
 *               不得对本文件进行部分或全部的拷贝，否则，本公司将依法追究相应的法律责任。
 * Description : 摄像机MP4格式视频播放器模块
 *=======================================================================================================================
 * Revision History:
 * 修改时间    修改人  修改内容
 * 2016-07-08  董超    新建
 * 2016-07-xx  董超    完成版本V1.0.0
 *=======================================================================================================================
 */
#ifndef _MP4PLAYER_H_
#define _MP4PLAYER_H_
#pragma once
#include "Global.h" 
#include "VideoPlayer.h" 
#include "plaympeg4.h"
#include "HCNetSDK.h"
#include "CameraCtrl.h"
#include "Buffer.h"

typedef enum 
{
    SW_HIK_PLAY_START              = 0,
    SW_HIK_PLAY_PAUSE              = 1,
    SW_HIK_PLAY_STOP               = 2,
    SW_HIK_PLAY_RESUME             = 3,
    SW_HIK_PLAY_SLOW_FWD           = 4,
    SW_HIK_PLAY_FAST_FWD           = 5,
    SW_HIK_PLAY_ONE_BY_ONE         = 6,
    SW_HIK_PLAY_SLOW_2X            = 7,
    SW_HIK_PLAY_SLOW_4X            = 8,
    SW_HIK_PLAY_FAST_2X            = 9,
    SW_HIK_PLAY_FAST_4X            = 10,
} SW_HIK_PLAY_CTRL_T;

typedef enum
{
    PLAY_MODE_START = 0,
    PLAY_MODE_PAUSE = 1,
} PLAY_MODE_T;

typedef enum
{
    ERR_FILE_NAME_INVALID         = -1,
    ERR_GET_PLAY_PORT_FAIL        = -2,   
    ERR_SET_STREAM_OPEN_MODE_FAIL = -3,
    ERR_OPEN_FILE_FAIL            = -4,
    ERR_START_PLAY_FAIL           = -5,
    
} PLAY_LOCAL_FILE_ERR_T;

typedef enum
{
    CH_INVALID              = -1,
    CH_PLAY_LOCAL_FILE      = 0,
    CH_PLAY_HIK_REAL_STREAM = 1,
    CH_PLAY_SWIFT_STREAM    = 2,
} PLAY_CHANNEL_T;

typedef enum
{
    PLAY_STATE_NOTPLAY      = 0,
    PLAY_STATE_PLAYING      = 1,
} PLAY_STATE_T;


class MP4Player : public VideoPlayer
{
public:
    MP4Player();
    virtual ~MP4Player();

private:
    CameraCtrl  *pSelectedCamera;
    Buffer      *pSelectedRealVideoBuffer;
    LONG        lPort;
	HWND        hWnd;
    LONG        lUserID;
    LONG        lRealPlayHandle;
    LPSTR       sHistoryFileName;
    LPSTR       sFileName;
    int         speedChangeVal;
    bool        isTerminted;        // 是否已经停止标志
    ulong       lastError;          // 线程运行至今最后一次出现的错误
    int         channelID;
    int         recordChannelID;
    int         playState;
protected:
  
public:
    void        Init(void);                                 // 播放器初始化
    void        SetCameraCtrl(CameraCtrl *pCameraCtrl);
    void        SetRealVideoBuffer(Buffer* realVideoBuffer);
    void        SaveData(Buffer *pBuffer, BYTE *pRcvData, int dataLen);
    void        SaveDataInit(void);
    void        SaveDataExit(void);
    void        SaveRealVideoData(BYTE *pRcvData, int dataLen);   
    void        SaveToFile(BYTE *pBuffer, DWORD dwBufSize);
    void        CALLBACK RealDataCallBack_V30(LONG lRealHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, DWORD dwUser);
    void        PlayRealData(DWORD dwDataType,BYTE *pBuffer, DWORD dwBufSize);
    void        RealPlayInit(LONG lUserID);           //开始实时预览
    void        RealPlayExit(void);                    //停止预览
    void        SetPlayLocalFileName(LPSTR sFileName);
    LONG        PlayLocalFile( LPSTR sFileName );      //播放本地文件
    void        PlayLocalFileExit(void);             //退出播放本地文件
    void        PlayLocalFileCtrl(void);            // 本地播放控制
    int         SetPlayChannel(int channelID);       //选择播放通道
    int         OpenChannel(int channelID);
    int         CloseChannel(int channelID);
    void        Play(void);                                // 正常速度播放
    void        Pause(void);                               // 暂停播放
    void        Resume(void);                   // 恢复正常速度播放
    void        Stop(void);                     // 停止播放
    void        PlayFast(void);                 // 按指定速度快速播放
    void        PlaySlow(void);                 // 按指定速度慢速播放
    void        PlayOneFrame(void);             // 播放一帧
    void        PlayOneByOne(void);             // 每秒播放一帧，连续播放
    void        PlayFastBack(void);             // 按指定速度快速倒退
    void        PlaySlowBack(void);             // 按指定速度慢速倒退
    void        PlayOneFrameBack(void);         // 倒退一帧
    void        PlayOneByOneBack(void);         // 每秒倒退一帧，连续播放
	void		SetConsoleWindow(HWND hWnd);
};
#endif
