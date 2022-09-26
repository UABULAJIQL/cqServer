#ifndef _CONNECTOBJ_H
#define _CONNECTOBJ_H

class Network;
class Packet;
class RecvNetworkBuffer;
class SendNetworkBuffer;

#include "disposable.h"


class ConnectObj : IDisposable {
    protected:
        const int _socket;
        Network *_network{nullptr};
        RecvNetworkBuffer *_recvBuf{nullptr};
        SendNetworkBuffer *_sendBuf{nullptr};

    public:
        ConnectObj(Network *network, int socket);

        //获取socket文件描述符
        int GetSocket() const;
        //接收缓冲区中是否有数据
        bool HasRecvData() const;
        //接收消息
        bool Recv() const;
        //发送缓冲区中是否有数据
        bool HasSendData() const;
        //发送消息
        bool Send() const;
        //获取接收缓冲区
        // RecvNetworkBuffer *GetRecvNetworkBuffer() const;
        //获取发送缓冲区
        // SendNetworkBuffer *GetSendNetworkBuffer() const;
        bool AddPacket(Packet* packet);
        Packet* GetPacket();

        void Dispose() override;
};

#endif
