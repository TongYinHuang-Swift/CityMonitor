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
uint _g_lPort = 0;           //ȫ�ֵĲ��ſ�port��
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
        case NET_DVR_SYSHEAD: //ϵͳͷ
        {
            if ( !PlayM4_GetPort((LONG *)(&_g_lPort)) )  //��ȡ���ſ�δʹ�õ�ͨ����
    		{
    			break;
    		}
    		//m_iPort = _g_lPort; //��һ�λص�����ϵͳͷ������ȡ�Ĳ��ſ�port�Ÿ�ֵ��ȫ��port���´λص�����ʱ��ʹ�ô�port�Ų���
    		if (dwBufSize > 0)
    		{
    			if ( !PlayM4_SetStreamOpenMode(_g_lPort, STREAME_REALTIME) )  //����ʵʱ������ģʽ
    			{
    				break;
    			}
                
    			if ( !PlayM4_OpenStream((int)_g_lPort, (PBYTE)pBuffer, (ULONG)dwBufSize, (ULONG)(1024 * 1024)) ) //�����ӿ�
    			{
    				break;
    			}
                #ifdef WIN32
    			if ( !PlayM4_Play(_g_lPort, hWnd) ) //���ſ�ʼ
    			{
    				break;
    			}
                #else
                if ( !PlayM4_Play(_g_lPort, NULL) ) //���ſ�ʼ
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
		
        case NET_DVR_STREAMDATA:   //��������
        {
            if ( dwBufSize > 0 && _g_lPort != -1 )
    		{
    			if ( !PlayM4_InputData(_g_lPort, pBuffer, dwBufSize) )
    			{
    				break;
    			} 
                #ifdef _SAVE_H264_STREAM
                FILE*       fpsave;            // ��д��Ƶ�ļ��������ļ�ָ��
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
		//д��ͷ����
		g_pFile = fopen("./record/ps.dat", "wb");

		if (g_pFile == NULL)
		{
			printf("CreateFileHead fail\n");
			return;
		}

		//д��ͷ����
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
        case EXCEPTION_RECONNECT:    //Ԥ��ʱ����
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
	    /* ע���û� */
	NET_DVR_Logout_V30(_g_lUserID);
	NET_DVR_Cleanup();
}

int SwiftHikSDK::SwHikSdkInit( void )
{
	/* ��ʼ����������ͷSDK */
	NET_DVR_Init();
	NET_DVR_SetLogToFile(3, "./record/");

#ifdef WIN32
    /* ע���豸 */
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

	/* �����쳣��Ϣ�ص����� */
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
    /* ע���û� */
    //NET_DVR_Logout_V30(_g_lUserID);
    //NET_DVR_Cleanup();
    return 0;
}

void SwiftHikSDK::SwHikRealplayStart( void )
{
    /* ����Ԥ�������ûص������� */
    NET_DVR_CLIENTINFO ClientInfo = {0};
    ClientInfo.hPlayWnd = NULL;         //��ҪSDK����ʱ�����Ϊ��Чֵ����ȡ��������ʱ����Ϊ��
    ClientInfo.lChannel     = 1;       //Ԥ��ͨ����
    ClientInfo.lLinkMode    = 0;       //���λ(31)Ϊ0��ʾ��������Ϊ1��ʾ������0��30λ��ʾ���ӷ�ʽ��0��TCP��ʽ��1��UDP��ʽ��2���ಥ��ʽ��3��RTP��ʽ;
    ClientInfo.sMultiCastIP = NULL;   //�ಥ��ַ����Ҫ�ಥԤ��ʱ����
    
    BOOL bPreviewBlock = false;       //�������������Ƿ�������0����1����
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
    /* �ر�Ԥ�� */
    NET_DVR_StopRealPlay(_g_lRealPlayHandle);
}



