#include "network.h"
#include "connectObj.h"

Network::Network() {
    _socket = socket(AF_INET, SOCK_STREAM, 0);
    if (_socket == -1)
        std::cout << "套接字创建失败\n";
}

void Network::Dispose() {
    //释放map集合
    /* auto iter = _connects.begin();
       while (iter != _connects.end()) {
       iter->second->Dispose();
       delete iter->second;
       ++iter;
       }
       _connects.clear();
       */
    //关闭文件描述符
    close(_socket);
    _socket = -1;
}


SOCKET Network::GetSocket() { return _socket; }

void Network::SetSocketOpt(SOCKET socket) {
    //启动端口复用
    bool isReuseAddr = true;
    setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, (void *)&isReuseAddr,
            sizeof(isReuseAddr));
    //设置套接字接收和发送的超时时间
    int netTimeout = 3000;
    setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, (void *)&netTimeout,
            sizeof(netTimeout));
    setsockopt(socket, SOL_SOCKET, SO_SNDTIMEO, (void *)&netTimeout,
            sizeof(netTimeout));

    // 设置文件描述符非阻塞
    int flags = fcntl(socket, F_GETFL, 0);
    fcntl(socket, F_SETFL, flags | O_NONBLOCK);

    /* //侦测包----不是使用心跳包代替
       int keepAlive = 1;
       socklen_t optlen = sizeof(keepAlive);

    //在socket 没有交互后 多久 开始发送侦测包
    int keepIdle = 60 * 2;
    //多次发送侦测包之间的间隔
    int keepInterval = 10;
    //侦测包个数
    int keepCount = 5;

    setsockopt(socket, SOL_SOCKET, SO_KEEPALIVE, (void *)&keepAlive, optlen);
    if (getsockopt(socket, SOL_SOCKET, SO_KEEPALIVE, &keepAlive, &optlen) < 0) {
    std::cout << "getsockopt SO_KEEPALIVE failed." << std::endl;
    }

    setsockopt(socket, SOL_TCP, TCP_KEEPIDLE, (void *)&keepIdle, optlen);
    if (getsockopt(socket, SOL_TCP, TCP_KEEPIDLE, &keepIdle, &optlen) < 0) {
    std::cout << "getsockopt TCP_KEEPIDLE failed." << std::endl;
    }

    setsockopt(socket, SOL_TCP, TCP_KEEPINTVL, (void *)&keepInterval, optlen);
    setsockopt(socket, SOL_TCP, TCP_KEEPCNT, (void *)&keepCount, optlen); */
}
