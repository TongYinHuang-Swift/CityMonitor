﻿/**
 * 多线程数据接收模块
 * 负责人：董超
 */
#include "Global.h"
#ifdef WIN32
#include <WinSock2.h>
#include "windows.h"
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<string.h>
#endif

#include<errno.h>
#include "Receiver.h" 
#include "Buffer.h" 
#include "Debugger.h" 

Receiver::Receiver()
{
	this->socket = 0;
	this->buffer = 0;
}
Receiver::Receiver(SOCKET socket, Buffer* buffer)
{
	this->socket = socket;
	this->buffer = buffer;

}
Receiver::~Receiver()
{
}
void Receiver::Start()
{
	while (0 != isTerminted)
	{
		/*
		If no error occurs, this function returns the number of bytes received. If the connection has been gracefully closed,
		the return value is zero. If an error occurs, a value of SOCKET_ERROR is returned, and a specific error code can be
		retrieved by calling WSAGetLastError
		*/
		//int len = recv( socket, (char*)recvBuffer, NUM_BYTES_TO_RECEIVE_EACH_TIME, MSG_OOB );	// 考虑每次应该接收多少字节？MSG_OOB or MSG_PEEK ?
		int len = recv( socket, (char*)recvBuffer, NUM_BYTES_TO_RECEIVE_EACH_TIME, 0 );	// 0为没有特殊效果
		if (len > 0)
		{
			uint i = 0;
			int result=0;
			PRINT(ALWAYS_PRINT, "Receiver", __FUNCTION__, __LINE__, "Received Data=%d",len);
			while ( len > 0 )
			{
				// 查询循环缓冲器剩余空间
				uint writelen = 0;
				uint availLen = buffer->GetUnusedSpace();
				if (availLen < len)
				{
					writelen = availLen;
				}
				else
				{
					writelen = len;
				}

				if (writelen > 0)
				{
					// 循环缓冲器本次可写入availLen个字节
					result=buffer->Write(recvBuffer + i, writelen);	// 写入数据
					PRINT(ALWAYS_PRINT, "Receiver", __FUNCTION__, __LINE__, "write len=%d,%d",writelen,result);

					len -= writelen;							// 计算下次可写入的字节数
					i  += writelen;							// 调整数据源地址指针
				}
				else
				{
#ifdef WIN32
					Sleep(100);
#else
					usleep(100000);
#endif
				}
			}
		}
		else
		{
#ifdef WIN32
			lastError = WSAGetLastError();

			switch (lastError)
			{
			case 0:					// The connection has been gracefully closed
				break;
			case WSAENETDOWN:		// The network subsystem has failed
				break;
			case WSAEFAULT:			// The buffer parameter is not completely contained in a valid part of the user address space
				break;
			case WSAENOTCONN:		// The socket is not connected
				break;
			case WSAEINTR:			// The socket was closed
				break;
			case WSAENETRESET:		// The connection has been broken due to the keep-alive activity detecting a failure while the operation was in progress
				break;
			case WSAESHUTDOWN:		// The socket has been shut down; it is not possible to receive on a socket after shutdown has been invoked with how set to SD_RECEIVE or SD_BOTH
				break;
			case WSAEWOULDBLOCK:	// The socket is marked as nonblocking and the receive operation would block
				break;
			case WSAEMSGSIZE:		// The message was too large to fit into the specified buffer and was truncated
				break;
			case WSAECONNABORTED:	// The virtual circuit was terminated due to a time-out or other failure. The application should close the socket as it is no longer usable
				break;
			case WSAETIMEDOUT:		// The connection has been dropped because of a network failure or because the peer system failed to respond
				break;
			case WSAECONNRESET:		// The virtual circuit was reset by the remote side executing a hard or abortive close. The application should close the socket because it is no longer usable
				break;
			case 10038:
				PRINT(ALWAYS_PRINT, "Receiver", __FUNCTION__, __LINE__, "receiveerror: %d %s\n",   lastError,"在一个非套接字上尝试了一个操作");
				break;
			}

			if (0 != lastError)
			{
				PRINT(ALWAYS_PRINT, "Receiver", __FUNCTION__, __LINE__, "receiveerror: %d \n",   lastError);
			}
#else
			lastError=errno;
			if (0 != lastError)
			{
				PRINT(ALWAYS_PRINT, "BasicTCP", __FUNCTION__, __LINE__, "receiveerror: %s \n",   strerror(errno));
			}
#endif
		}
	}
/*
#else
    uint len = read(socket, (char*)recvBuffer, NUM_BYTES_TO_RECEIVE_EACH_TIME);	// 
		if (len > 0)
		{
			uint i = 0;
			// 将本次接收到的数据存入循环缓冲器 buffer 中
			while ( len > 0 )
			{
				// 查询循环缓冲器剩余空间
				uint availLen = buffer->GetUnusedSpace();
				if (availLen > 0)
				{
					// 循环缓冲器本次可写入availLen个字节
					buffer->Write(recvBuffer + i, availLen);	// 写入数据
					len -= availLen;							// 计算下次可写入的字节数
					i  += availLen;							// 调整数据源地址指针
				}
			}
		}
		else if(len==0)
 {  
			  break;//连接断开
		} else { 
			break; //出错errno
		}
	}

#endif
*/
}
