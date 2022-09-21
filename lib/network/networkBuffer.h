#ifndef _RECVNETWORKBUFFER_H
#define _RECVNETWORKBUFFER_H
#include "circularQueueBuffer.h"

class Packet;

const unsigned int RECVBUF_DEFAULT_SIZE = 64;
const unsigned int SENDBUF_DEFAULT_SIZE = 64;
//小测 ok

//接收
class RecvNetworkBuffer : public CircularQueueBuffer {
  public:
    explicit RecvNetworkBuffer(unsigned int size);
    RecvNetworkBuffer();

    //从接收缓冲区中获取一个packet
    Packet* GetPacket();

    //获取缓冲区地址 返回尾部下标右边的空间长度
    unsigned int GetBuffer(char *&buf) const;
    //添加数据后尾部下标更新
    void ChangeEndIndex(int size);
};

//发送缓冲区
class SendNetworkBuffer : public CircularQueueBuffer {
  public:
    explicit SendNetworkBuffer(unsigned int size);
    SendNetworkBuffer();

    //添加一个Packet到发送缓冲区中
    bool AddPacket(Packet* p);

    //获取缓冲区地址 返头部下标右边的空间长度
    unsigned int GetBuffer(char *&buf) const;
    //移除数据化头部下标更新
    void ChangeBeginIndex(int size);

};

#endif
