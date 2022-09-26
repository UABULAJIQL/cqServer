#ifndef _CLIENT_H
#define _CLIENT_H

#include "network/epoll/epollClient.h"
#include <random>
#include <thread>
class Client : public EpollClient {
    public:
        explicit Client(int msgCount, std::thread::id threadId);
        //连接成后将消息清空
        bool Connect(std::string ip, int port) override;
        //发送和接收数据
        void DataHandler();
        //是否完成
        bool IsCompleted() const;

        void Dispose() override;

    private:
        //随机字符串
        std::string GenRandowm();

    private:
        //要发送消息的总体数量
        int _msgCount{0};
        //当前消息的序号
        int _index{0};
        //是否发送完成
        bool _isCompleted{false};
        //临时消息
        std::string _lastMsg{""};
        //随机引擎
        std::default_random_engine *_pRandomEngine;
};

#endif
