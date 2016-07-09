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

#ifdef __cplusplus
extern "C" {
#endif

void CALLBACK g_RealDataCallBack_V30(LONG lRealHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, DWORD dwUser);
void CALLBACK g_ExceptionCallBack(DWORD dwType, LONG lUserID, LONG lHandle, void *pUser);

#ifdef __cplusplus
    }
#endif /* ENDIF __cplusplus */


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
    DISPLAY_ENABLE  = 0,
    DISPLAY_DISABLE = 1,
} PLAY_ON_WINDOW_EN_T;

class MP4Player : public VideoPlayer
{
public:
    MP4Player();
    virtual ~MP4Player();

private:
    LONG lPort;
    LONG lUserID;
    LONG lRealPlayHandle;
    LPSTR sHistoryFileName;
    int speedChangeVal;
protected:
  
public:
    void    Init(void);                                 // ��������ʼ��
    void    RealPlayStart(LONG lUserID);           //��ʼʵʱԤ��
    void    RealPlayStop(void);                    //ֹͣԤ��
    void    SetPlayOnWindow(int playOnWin);
    int     GetPlayOnWindow(void);
    LONG    PlayLocalFile( LPSTR sFileName );      //���ű����ļ�
    void    PlayLocalFileExit(void);             //�˳����ű����ļ�
    void    PlayLocalFileCtrl(void);            // ���ز��ſ���
    void    Play(void);                                // �����ٶȲ���
    void    Pause(void);                               // ��ͣ����
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
