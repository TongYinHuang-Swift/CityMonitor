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
    bool        isTerminted;        // �Ƿ��Ѿ�ֹͣ��־
    ulong       lastError;          // �߳������������һ�γ��ֵĴ���
    int         channelID;
    int         recordChannelID;
    int         playState;
protected:
  
public:
    void        Init(void);                                 // ��������ʼ��
    void        SetCameraCtrl(CameraCtrl *pCameraCtrl);
    void        SetRealVideoBuffer(Buffer* realVideoBuffer);
    void        SaveData(Buffer *pBuffer, BYTE *pRcvData, int dataLen);
    void        SaveDataInit(void);
    void        SaveDataExit(void);
    void        SaveRealVideoData(BYTE *pRcvData, int dataLen);   
    void        SaveToFile(BYTE *pBuffer, DWORD dwBufSize);
    void        CALLBACK RealDataCallBack_V30(LONG lRealHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, DWORD dwUser);
    void        PlayRealData(DWORD dwDataType,BYTE *pBuffer, DWORD dwBufSize);
    void        RealPlayInit(LONG lUserID);           //��ʼʵʱԤ��
    void        RealPlayExit(void);                    //ֹͣԤ��
    void        SetPlayLocalFileName(LPSTR sFileName);
    LONG        PlayLocalFile( LPSTR sFileName );      //���ű����ļ�
    void        PlayLocalFileExit(void);             //�˳����ű����ļ�
    void        PlayLocalFileCtrl(void);            // ���ز��ſ���
    int         SetPlayChannel(int channelID);       //ѡ�񲥷�ͨ��
    int         OpenChannel(int channelID);
    int         CloseChannel(int channelID);
    void        Play(void);                                // �����ٶȲ���
    void        Pause(void);                               // ��ͣ����
    void        Resume(void);                   // �ָ������ٶȲ���
    void        Stop(void);                     // ֹͣ����
    void        PlayFast(void);                 // ��ָ���ٶȿ��ٲ���
    void        PlaySlow(void);                 // ��ָ���ٶ����ٲ���
    void        PlayOneFrame(void);             // ����һ֡
    void        PlayOneByOne(void);             // ÿ�벥��һ֡����������
    void        PlayFastBack(void);             // ��ָ���ٶȿ��ٵ���
    void        PlaySlowBack(void);             // ��ָ���ٶ����ٵ���
    void        PlayOneFrameBack(void);         // ����һ֡
    void        PlayOneByOneBack(void);         // ÿ�뵹��һ֡����������
	void		SetConsoleWindow(HWND hWnd);
};
#endif
