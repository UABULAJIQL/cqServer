#ifndef _NETWORK_H
#define _NETWORK_H

#include "tools/disposable.h"
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

// socket文件描述符别名
using SOCKET = int;

class ConnectObj;
class Network : public IDisposable {
    protected:
        //文件描述符
        SOCKET _socket = {-1};

    public:
        //构造
        Network();
        //释放
        void Dispose() override;
        //获取套接字文件描述符
        SOCKET GetSocket();

    protected:
        //设置端口复用、接收发送超时时间
        static void SetSocketOpt(SOCKET socket);
};

#endif
