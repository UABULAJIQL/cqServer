#ifndef _RECVNETWORKBUFFER_H
#define _RECVNETWORKBUFFER_H
#include "circularQueueBuffer.h"

class Packet;

// 默认大小1k
const unsigned int RECVBUF_DEFAULT_SIZE = 1024;
const unsigned int SENDBUF_DEFAULT_SIZE = 1024;
// 小测 ok

class ConnectObj;

// 接收
class RecvNetworkBuffer : public CircularQueueBuffer {
  protected:
    ConnectObj *_fConnectObj;

  public:
    explicit RecvNetworkBuffer(ConnectObj *connectObj,
                               unsigned int size = RECVBUF_DEFAULT_SIZE);

    // 从接收缓冲区中获取一个packet
    Packet *GetPacket();

    // 获取缓冲区地址 返回尾部下标右边的空间长度
    unsigned int GetBuffer(char *&buf) const;
    char *GetBuffer() const;
    // 添加数据后尾部下标更新
    void ChangeEndIndex(int size);
};

// 发送缓冲区
class SendNetworkBuffer : public CircularQueueBuffer {
  protected:
    ConnectObj *_fConnectObj;

  public:
    explicit SendNetworkBuffer(ConnectObj *connectObj,
                               unsigned int size = SENDBUF_DEFAULT_SIZE);

    // 添加一个Packet到发送缓冲区中
    bool AddPacket(Packet *p);

    // 获取缓冲区地址 返头部下标右边的空间长度
    unsigned int GetBuffer(char *&buf) const;
    // 移除数据化头部下标更新
    void ChangeBeginIndex(int size);
};

#endif
