#ifndef _NETWORKCONNECTOR_H
#define _NETWORKCONNECTOR_H

#include "network.h"

class Packet;

class NetworkConnector : public Network {
    protected:
        std::string _ip{""};
        int _port{0};
        ConnectObj *_connectObj{nullptr};

    public:
        //连接
        bool Connect(std::string ip, int port);
        //判断是否连接成功
        bool IsConnected() const;
        //获取connectObj
        ConnectObj *GetConnectObj() const;

        //添加一个Packet到发送缓冲区中
        bool AddPacket(Packet *p);
        //从接收缓冲区中获取一个packet
        Packet *GetPacket();

        //帧
        virtual bool Update() = 0;

        //有虚函数 所以要有虚析构
        virtual ~NetworkConnector() = default;

        void Dispose() override;
};

#endif
