#include "VideoManager.h"
#include "HistoryVideoPlayer.h" 
#include "VideoRecorder.h" 
#include "TcpConn.h" 
#include "Buffer.h" 
#include "Codec.h" 
#include "DateTime.h" 
#include "Debugger.h" 
#include <cstring>

#include "CameraCtrl.h"
#include "Camera.h"
#include "VideoPlayer.h"
#include "MP4Player.h"

#ifndef WIN32
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>    // for socket
#include <netinet/in.h>
#include <signal.h>
#include <assert.h>
#include <linux/if.h>
#include <sys/ioctl.h>
#endif


VideoManager::VideoManager(Camera* camera)
{
    this->camera = camera;
    Init();
}
VideoManager::~VideoManager()
{
    ReleaseMemory();
}
/**
 * �����Ƶ�������ĳ�ʼ������ʼ��������
 * 1. ��������ͨ���������Ľ���
 * 2. ��������������Ӧ��������Ϣͨ���Ľ���
 * 3. ������������ݻ�ȡ�������Ľ���
 * 4. �������������ͨ���Ľ���
 * 5. ��Ƶ����������������Ľ���
 * 6. ������Ƶ¼������ܵĴ���
 * 7. ��������������
 * 8. ������ʷ��Ƶ������
 * �����Ƶת�����ܵĿͻ�������Ƶ����������������ڲ���̬������������ÿ����һ�����ڿͻ��˾Ͷ�̬�ش���һ�����ؿͻ��������ڷ�����������
 * ��ɱ�����Ƶ���Ź��ܵĿͻ�����Codec������������ʱ������
 */
void VideoManager::Init()
{
    debugger = new Debugger();
    GetLocalIpAddr();
    GetLeftNodeIpAddr(localIpAddr);
    GetRightNodeIpAddr(localIpAddr);

    cmdBuffer  = new Buffer(CMD_BUFFER_LEN, CMD_BUFFER_ID);
    cmdAckBuffer = new Buffer(CMD_BUFFER_LEN, CMD_ACK_BUFFER_ID);
    videoRecordBuffer = new Buffer(VIDEO_BUFFER_LEN, VIDEO_BUFFER_ID);
    cameraCtrlBuffer  = new Buffer(CAM_CTRL_BUFFER_LEN, CAM_CTRL_BUFFER_ID);
    PRINT(ALWAYS_PRINT, "VideoManager",  __FUNCTION__, __LINE__, " leftNodeAddr = %s, localIpAddr = %s, rightNodeAddr = %s", leftNodeAddr, localIpAddr, rightNodeAddr);

    // ���������������ڽڵ����������ķ�����
    cmdServer4LeftNode = new TcpConn(MODE_SERVER, COMMAND_CHAN_CONNECTION, this, cmdAckBuffer, cmdBuffer);
    cmdServer4LeftNode->SetLocalInfo(localIpAddr, CMD_RELAY_CONN_PORT);
    // ����TCP/IP������
    cmdServer4LeftNode->CreateServer();

//  #ifndef NO_RIGHT_SERVER
    // �������������ڽڵ����������ת�������ڽڵ�Ŀͻ���
    cmdClient4RightNode = new TcpConn(MODE_CLIENT, COMMAND_CHAN_CONNECTION, this, cmdBuffer, cmdAckBuffer);
    cmdClient4RightNode->SetLocalInfo(localIpAddr);
    cmdClient4RightNode->SetRemoteInfo(rightNodeAddr, CMD_RELAY_CONN_PORT);
    // ����TCP/IP�ͻ���
    cmdClient4RightNode->CreateClient();
//  #endif

    // ��������������
    codec = new Codec(this,cmdBuffer);
    codec->Init();

    // ���������������Ƶ���ݽ��շ��������������ǲ�֪�������ӵ������IP��ַ������ֻ��ͨ���������ķ�ʽ��ȡ��Ƶ���ݣ��������ͨ��push�ķ�ʽ�������ݣ�
    videoServer4LocalCam = new TcpConn(MODE_SERVER, LOCAL_CAMERA_CONNECTION, this, cameraCtrlBuffer, videoRecordBuffer);
    videoServer4LocalCam->SetLocalInfo(localIpAddr, LOCAL_CAMERA_CONN_PORT);
    // ����TCP/IP������
    videoServer4LocalCam->CreateServer4Cam();

    // ���������������Ƶ¼���
    videoRecorder = new VideoRecorder(videoRecordBuffer);

    // ���������������ڽڵ���Ƶ���ķ�������ת����Ƶ�����ڽڵ�Ŀͻ����ڸ÷�������Acceptor�߳��д�����
    videoServer4RightNode = new TcpConn(MODE_SERVER, RIGHT_NODE_CONNECTION, this);
    videoServer4RightNode->SetLocalInfo(localIpAddr, VIDEO_RELAY_CONN_PORT);
    // ����TCP/IP������
    videoServer4RightNode->CreateServer();


#define TEST_REAL_PLAY_0
#ifdef TEST_REAL_PLAY_0
    //SwiftHikSDK *hikSdk = new SwiftHikSDK();
    CameraCtrl *CamTest = new CameraCtrl();

    CamTest->Init();
    
    LPNET_DVR_DEVICEINFO_V30 DeviceInfo;
    memset( &DeviceInfo, 0, sizeof(LPNET_DVR_DEVICEINFO_V30) );
    LONG lUserID = CamTest->Login( "192.168.1.65" );
    MP4Player *MP4PlayTest = new MP4Player();
    MP4PlayTest->SetCameraCtrl(CamTest);
    MP4PlayTest->Init();
#ifdef WIN32
    MP4PlayTest->SetConsoleWindow(GetConsoleWindow());
#endif
    MP4PlayTest->SetRealVideoBuffer(cameraCtrlBuffer);
    
    if ( lUserID != -1 )
    {
        MP4PlayTest->RealPlayInit(lUserID);
    }
#endif

//#define TEST_REAL_PLAY_1

#ifdef TEST_REAL_PLAY_1
    CameraCtrl* cameraCtrl = new CameraCtrl();
    VideoPlayer* videoPlayer = new VideoPlayer();  
    Camera *Cam = new Camera(cameraCtrl, videoPlayer);
    
    Cam->Init();
    LPNET_DVR_DEVICEINFO_V30 DeviceInfo;
    memset( &DeviceInfo, 0, sizeof(LPNET_DVR_DEVICEINFO_V30) );
    LONG lUserID = Cam->Login("192.168.1.65", 8000, "admin", "admin0123", DeviceInfo);
    
    MP4Player *MP4PlayTest = new MP4Player();
    MP4PlayTest->SetCameraCtrl(cameraCtrl);
    MP4PlayTest->Init();
    MP4PlayTest->SetPlayOnWindow(DISPLAY_ENABLE);
    
    if ( lUserID != -1 )
    {
        MP4PlayTest->RealPlayInit(lUserID);
    }
    delete MP4PlayTest;
    delete videoPlayer;
    delete cameraCtrl;
#endif

    //#define TEST_PLAY_LOCAL_FILE
#ifdef TEST_PLAY_LOCAL_FILE
    MP4Player *MP4PlayTest = new MP4Player();
    MP4PlayTest->Init();
    MP4PlayTest->SetPlayOnWindow(DISPLAY_DISABLE);
    MP4PlayTest->PlayLocalFile("test.record");
    MP4PlayTest->PlayLocalFileCtrl();
    delete MP4PlayTest;
#endif

}
void VideoManager::Run()
{
    while (1)
    {
        #ifdef WIN32
            Sleep(1000);
        #else
            sleep(1000);
        #endif
    }
}
/**
 * �������ϵͳ��������ִ�����
 * ���ϵͳ��������������ģ������������ͷ��ڴ棬Ȼ���ؽ�ϵͳ��
 */
void VideoManager::SystemRestart()
{
    PRINT(ALWAYS_PRINT, "VideoManager",  __FUNCTION__, __LINE__, " %s", localIpAddr);
    Reset();            // �ȸ�λVideoManager

    // TODO
    // ����ģ����ȸ�λ�ڴ����
}
/**
 * �������ϵͳʵʱ��Ƶֱ������ִ�����
 */
void VideoManager::PlayRealTimeVideo()
{
    PRINT(ALWAYS_PRINT, "VideoManager",  __FUNCTION__, __LINE__, " %s", localIpAddr);
    // TODO
    // ���������ڽڵ�����������Ŀͻ������ӷ��ͱ���ʵʱ��Ƶ
    videoClient4LeftNode = new TcpConn(MODE_CLIENT, COMMAND_CHAN_CONNECTION, this, cmdBuffer, cmdAckBuffer);
    videoClient4LeftNode->SetLocalInfo(localIpAddr);
    videoClient4LeftNode->SetRemoteInfo(rightNodeAddr, CMD_RELAY_CONN_PORT);
    // ����TCP/IP�ͻ���
    videoClient4LeftNode->CreateClient();
}
// ������Ľ���ʵʱ��Ƶֱ������ִ�����
void VideoManager::StopPlayRealTimeVideo()
{
    PRINT(ALWAYS_PRINT, "VideoManager",  __FUNCTION__, __LINE__, " %s", localIpAddr);
}
/**
 * ������ĵ㲥��ʷ��Ƶ����ִ�����
 */
void VideoManager::PlayHistoryVideo(DateTime* startTime, DateTime* endTime, byte mode, byte speed)
{
    PRINT(ALWAYS_PRINT, "VideoManager",  __FUNCTION__, __LINE__, " %s", localIpAddr);
    // TODO
    // ��HistoryVideoPlayer�д���һ��VideoFileReader��һ�������ڽڵ�����������Ŀͻ�������
}
// ������Ľ����㲥��ʷ��Ƶ����ִ�����
void VideoManager::StopPlayHistoryVideo(DateTime* startTime, DateTime* endTime)
{
    PRINT(ALWAYS_PRINT, "VideoManager",  __FUNCTION__, __LINE__, " %s", localIpAddr);
}
/**
 * ������Ļ�ȡ¼���б�����ִ�����
 */
void VideoManager::SendHistoyVideoList()
{
    PRINT(ALWAYS_PRINT, "VideoManager",  __FUNCTION__, __LINE__, " %s", localIpAddr);
    // TODO
    // ��HistoryVideoManager��ȡ¼���б�������¼���б�д��cmdAckBuffer
}
/**
 * �ͷ��ڴ档
 */
void VideoManager::ReleaseMemory()
{
    PRINT(ALWAYS_PRINT, "VideoManager",  __FUNCTION__, __LINE__, " %s", localIpAddr);
    delete debugger;
    delete videoRecorder;
    delete cmdClient4RightNode;
    delete cmdServer4LeftNode;
    delete videoServer4LocalCam;
    delete videoServer4RightNode;
    delete codec;
    delete cmdBuffer;
    delete cmdAckBuffer;
    delete cameraCtrlBuffer;
    delete videoSendBuffer;
    delete videoRecordBuffer;
}
/**
 * ���VideoManager��ʼ�������ͷ��ڴ棬Ȼ���ؽ���
 */
void VideoManager::Reset()
{
    PRINT(ALWAYS_PRINT, "VideoManager",  __FUNCTION__, __LINE__, " %s", localIpAddr);
    //ReleaseMemory();
    //Init();
}
// ��ȡ���ڽڵ��IP��ַ
void VideoManager::GetLeftNodeIpAddr(char* localIPAddress)
{
    PRINT(ALWAYS_PRINT, "VideoManager", __FUNCTION__, __LINE__, " localIpAddr = %s, LeftNodeIpAddr = %s", localIpAddr, LEFTNODE_IP_ADDR);
    strcpy(leftNodeAddr, LEFTNODE_IP_ADDR);
}
// ��ȡ���ڽڵ��IP��ַ
void VideoManager::GetRightNodeIpAddr(char* localIPAddress)
{
    PRINT(ALWAYS_PRINT, "VideoManager", __FUNCTION__, __LINE__, " localIpAddr = %s, RightNodeIpAddr = %s", localIpAddr, RIGHTNODE_IP_ADDR);
    strcpy(rightNodeAddr, RIGHTNODE_IP_ADDR);
}

// ��ȡ������IP��ַ��ע�⣺���� "127.0.0.1"��
void VideoManager::GetLocalIpAddr()
{
    PRINT(ALWAYS_PRINT, "VideoManager", __FUNCTION__, __LINE__, " %s", localIpAddr);
#ifdef WIN32
    strcpy(localIpAddr, LOCAL_IP_ADDR);
#else
    int sock_get_ip;
    //char ipaddr[50];
    struct   sockaddr_in *sin;
    struct   ifreq ifr_ip;
    if ((sock_get_ip=socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        PRINT(ALWAYS_PRINT, "VideoManager", __FUNCTION__, __LINE__, "socket open error");
        strcpy(localIpAddr, "Invalid IP Addr");
        return;
    }
    memset(&ifr_ip, 0, sizeof(ifr_ip));
    strncpy(ifr_ip.ifr_name, (const char *)"eth0", sizeof(ifr_ip.ifr_name) - 1);
    if( ioctl( sock_get_ip, SIOCGIFADDR, &ifr_ip) < 0 )
    {
        PRINT(ALWAYS_PRINT, "VideoManager", __FUNCTION__, __LINE__, "ioctrl  error");
        strcpy(localIpAddr, "Invalid IP Addr");
        return;
    }
    sin = (struct sockaddr_in *)&ifr_ip.ifr_addr;
    strcpy(localIpAddr,inet_ntoa(sin->sin_addr));
    close( sock_get_ip );
    PRINT(ALWAYS_PRINT, "VideoManager", __FUNCTION__, __LINE__, "localIpAddr = %s",localIpAddr);
#endif
}


