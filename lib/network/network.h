#ifndef _NETWORK_H
#define _NETWORK_H

#include "thread/threadObject.h"

#include <iostream>

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/types.h>

// #include <map>
#include "tools/socketObj.h"

// socket文件描述符别名
using SOCKET = int;

class ConnectObj;
class Network : public ThreadObject, public ISocketObject {
    protected:
        //文件描述符
        SOCKET _socket = {-1};

        // 发送协议
        std::mutex _sendMsgMutex;
        std::list<Packet *> _sendMsgList;

    public:
        //构造
        Network();

        //注册消息协议
        void RegisterMsgFunction() override;

        //发送packet
        void SendPacket(Packet *);


        //释放
        void Dispose() override;
        //获取套接字文件描述符
        SOCKET GetSocket() override;

    protected:
        //设置端口复用、接收发送超时时间
        static void SetSocketOpt(SOCKET socket);

        //断开连接协议处理函数
        virtual void HandleDisconnect(Packet *pPacket) = 0;
};

#endif
