/*
 * Copyright (c) 2015-2016 Swift-tech.Inc. All rights reserved.
 *
 * Swift-tech retains all right, title and interest (including all
 * intellectual property rights) in and to this computer program, which is
 * protected by applicable intellectual property laws.  Unless you have
 * obtained a separate written license from Swift-tech., you are not
 * authorized to utilize all or a part of this computer program for any
 * purpose (including reproduction, distribution, modification, and
 * compilation into object code), and you must immediately destroy or
 * return to Swift-tech all copies of this computer program.  If you
 * are licensed by Swift-tech, your rights to utilize this computer
 * program are limited by the terms of that license.  To obtain a license,
 * please contact Swift-tech.
 *
 * This computer program contains trade secrets owned by Swift-tech.
 * and, unless unauthorized by Swift-tech in writing, you agree to
 * maintain the confidentiality of this computer program and related
 * information and to not disclose this computer program and related
 * information to any other person or entity.
 *
 * THIS COMPUTER PROGRAM IS PROVIDED AS IS WITHOUT ANY WARRANTIES, AND
 * SWIFT-TECH EXPRESSLY DISCLAIMS ALL WARRANTIES, EXPRESS OR IMPLIED,
 * INCLUDING THE WARRANTIES OF MERCHANTIBILITY, FITNESS FOR A PARTICULAR
 * PURPOSE, TITLE, AND NONINFRINGEMENT.
 */
 /*
  * File   : SwiftHikSDK.cpp
  * Author : TonyHuang
  * Date   : 2016.07
  */

#include <stdio.h>
#include <iostream>
#include <time.h>

#include "SwiftHikSDK.h"
#include "Debugger.h"
#include "HCNetSDK.h"
#include "plaympeg4.h"
#include "DataType.h"

using namespace std;

uint _g_lRealPlayHandle = 0;
uint _g_lPort = 0;           //全局的播放库port号
uint _g_lUserID = 0;


void CALLBACK g_RealDataCallBack_V30(LONG lRealHandle, DWORD dwDataType, BYTE *pBuffer,DWORD dwBufSize,DWORD dwUser)
{

//	printf("HIKSDK buffer size=%d,%d,%d\n",dwDataType,dwBufSize,dwUser);

#ifdef WIN32
    HWND hWnd = GetConsoleWindow();
#endif
#if 1
	switch (dwDataType)
	{
        case NET_DVR_SYSHEAD: //系统头
        {
            if ( !PlayM4_GetPort((LONG *)(&_g_lPort)) )  //获取播放库未使用的通道号
    		{
    			break;
    		}
    		//m_iPort = _g_lPort; //第一次回调的是系统头，将获取的播放库port号赋值给全局port，下次回调数据时即使用此port号播放
    		if (dwBufSize > 0)
    		{
    			if ( !PlayM4_SetStreamOpenMode(_g_lPort, STREAME_REALTIME) )  //设置实时流播放模式
    			{
    				break;
    			}
                
    			if ( !PlayM4_OpenStream((int)_g_lPort, (PBYTE)pBuffer, (ULONG)dwBufSize, (ULONG)(1024 * 1024)) ) //打开流接口
    			{
    				break;
    			}
                #ifdef WIN32
    			if ( !PlayM4_Play(_g_lPort, hWnd) ) //播放开始
    			{
    				break;
    			}
                #else
                if ( !PlayM4_Play(_g_lPort, NULL) ) //播放开始
    			{
    				break;
    			}
                #endif
                
                #ifdef _PRINT_HEXDATA
                int i = 0;
                for ( i = 0; i < 32; i++ )
                {
                    if ( dwBufSize < 32 || pBuffer == NULL )
                    {
                        break;
                    }
                    printf("%d", pBuffer[i]);
                }
                #endif /* ENDIF _PRINT_HEXDATA */

                #ifdef _SAVE_H264_STREAM
                FILE*       fpsave;
                fpsave = fopen("test.record", "ab+");

            	if ( fpsave == NULL )
            	{
        			printf("err:open file failed\n");
            		return;
            	}
                fwrite(pBuffer, dwBufSize, 1, fpsave);
                fclose(fpsave);
                #endif /* ENDIF _SAVE_H264_STREAM */
    		}
        }
		
        case NET_DVR_STREAMDATA:   //码流数据
        {
            if ( dwBufSize > 0 && _g_lPort != -1 )
    		{
    			if ( !PlayM4_InputData(_g_lPort, pBuffer, dwBufSize) )
    			{
    				break;
    			} 
                #ifdef _SAVE_H264_STREAM
                FILE*       fpsave;            // 待写视频文件的索引文件指针
                fpsave = fopen("test.record", "ab+");

            	if ( fpsave == NULL )
            	{
    				printf("err:open file failed\n");
            		return;
            	}
                fwrite(pBuffer, dwBufSize, 1, fpsave);
                fclose(fpsave);
                #endif
    		}
        }
        
        default:
        break;
	}
#endif
}
FILE *g_pFile = NULL;

void PsDataCallBack(LONG lRealHandle, DWORD dwDataType,BYTE *pPacketBuffer,DWORD nPacketSize, void* pUser)
{

	if (dwDataType  == NET_DVR_SYSHEAD)
	{
		//写入头数据
		g_pFile = fopen("./record/ps.dat", "wb");

		if (g_pFile == NULL)
		{
			printf("CreateFileHead fail\n");
			return;
		}

		//写入头数据
		fwrite(pPacketBuffer, sizeof(unsigned char), nPacketSize, g_pFile);
		printf("write head len=%d\n", nPacketSize);
	}
	else
	{
		if(g_pFile != NULL)
		{
			fwrite(pPacketBuffer, sizeof(unsigned char), nPacketSize, g_pFile);
			printf("write data len=%d\n", nPacketSize);
		}
	}

}

void CALLBACK g_ExceptionCallBack(DWORD dwType, LONG lUserID, LONG lHandle, void *pUser)
{
    char tempbuf[256] = {0};
    
    switch(dwType) 
    {
        case EXCEPTION_RECONNECT:    //预览时重连
        //PRINT(ALWAYS_PRINT, "SwiftHikSDK", __FUNCTION__, __LINE__,"Reconnect=%d",time(NULL));
        break;
    	default:
        break;
    }
}

SwiftHikSDK::SwiftHikSDK(void)
{
	SwHikSdkInit();
}
SwiftHikSDK::~SwiftHikSDK(void)
{
	SwHikRealplayStop();
	    /* 注销用户 */
	NET_DVR_Logout_V30(_g_lUserID);
	NET_DVR_Cleanup();
}

int SwiftHikSDK::SwHikSdkInit( void )
{
	/* 初始化海康摄像头SDK */
	NET_DVR_Init();
	NET_DVR_SetLogToFile(3, "./record/");

#ifdef WIN32
    /* 注册设备 */
	NET_DVR_DEVICEINFO_V30 struDeviceInfo;
    
    _g_lUserID = NET_DVR_Login_V30("192.168.1.65", 8000, "admin", "admin0123", &struDeviceInfo);
    if (_g_lUserID < 0)
    {
    	PRINT(ALWAYS_PRINT, "SwiftHikSDK", __FUNCTION__, __LINE__,"Login Error=%d",NET_DVR_GetLastError());
    	NET_DVR_Cleanup();
    	return -1;
    }
    else
    {
    	PRINT(ALWAYS_PRINT, "SwiftHikSDK", __FUNCTION__, __LINE__,"Login Sucess");
    }

	/* 设置异常消息回调函数 */
	NET_DVR_SetExceptionCallBack_V30(0, NULL, g_ExceptionCallBack, NULL);

	SwHikRealplayStart();
#else
	NET_DVR_DEVICEINFO_V30 struDeviceInfo;
	int iUserID = NET_DVR_Login_V30("192.168.1.65", 8000, "admin", "admin0123", &struDeviceInfo);
	if(iUserID >= 0)
	{
		NET_DVR_PREVIEWINFO struPreviewInfo = {0};
		struPreviewInfo.lChannel = 1;
		struPreviewInfo.dwStreamType = 0;
		struPreviewInfo.dwLinkMode = 0;
		struPreviewInfo.bBlocked = 1;
		struPreviewInfo.bPassbackRecord  = 1;
		int iRealPlayHandle = NET_DVR_RealPlay_V40(iUserID, &struPreviewInfo, PsDataCallBack, NULL);
		if(iRealPlayHandle >= 0)
		{
			printf("[SwiftHikSDK]---RealPlay %d success, \n", NET_DVR_GetLastError());
		}
		else
		{
			printf("[SwiftHikSDK]---RealPlay failed, error = %d\n", NET_DVR_GetLastError());
		}
	}
	else
	{
		printf("[SwiftHikSDK]---Login failed, error = %d\n", NET_DVR_GetLastError());
	}
#endif
    //system("PAUSE");
    
    //SwHikRealplayStop();
    /* 注销用户 */
    //NET_DVR_Logout_V30(_g_lUserID);
    //NET_DVR_Cleanup();
    return 0;
}

void SwiftHikSDK::SwHikRealplayStart( void )
{
    /* 启动预览并设置回调数据流 */
    NET_DVR_CLIENTINFO ClientInfo = {0};
    ClientInfo.hPlayWnd = NULL;         //需要SDK解码时句柄设为有效值，仅取流不解码时可设为空
    ClientInfo.lChannel     = 1;       //预览通道号
    ClientInfo.lLinkMode    = 0;       //最高位(31)为0表示主码流，为1表示子码流0～30位表示连接方式：0－TCP方式；1－UDP方式；2－多播方式；3－RTP方式;
    ClientInfo.sMultiCastIP = NULL;   //多播地址，需要多播预览时配置
    
    BOOL bPreviewBlock = false;       //请求码流过程是否阻塞，0：否，1：是
    _g_lRealPlayHandle = NET_DVR_RealPlay_V30(_g_lUserID, &ClientInfo, NULL, NULL, 0);

    if (_g_lRealPlayHandle < 0)
    {
        PRINT(ALWAYS_PRINT, "SwiftHikSDK", __FUNCTION__, __LINE__,"NET_DVR_RealPlay_V30 error");
        NET_DVR_Logout(_g_lUserID);
        NET_DVR_Cleanup();
        return;
    }

    if ( !NET_DVR_SetRealDataCallBack(_g_lRealPlayHandle, g_RealDataCallBack_V30, 0) )
    {
        PRINT(ALWAYS_PRINT, "SwiftHikSDK", __FUNCTION__, __LINE__,"NET_DVR_SetRealDataCallBack error=%d",NET_DVR_GetLastError());
    }
}

void SwiftHikSDK::SwHikRealplayStop( void )
{
    /* 关闭预览 */
    NET_DVR_StopRealPlay(_g_lRealPlayHandle);
}



