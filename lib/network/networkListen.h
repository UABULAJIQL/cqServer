#ifndef _NETWORKLISTEN_H
#define _NETWORKLISTEN_H
#include "network.h"
#include <map>
#include <string>
class NetworkListen : public Network {
  protected:
    // 连接集合
    std::map<SOCKET, ConnectObj *> _connects;

  public:
    // 设置绑定ip和端口然后设置监听
    bool Listen(int prot, std::string ip);

    // 监听是请求
    //  virtual bool Update() = 0;

    void Dispose() override;

  protected:
    // 获取连接 添加到集合中 返回值是添加的连接个数
    virtual int Accept() = 0;

    // 创建connectObj 添加到map集合中
    bool CreateConnectObj(SOCKET socket);

    void HandleDisconnect(Packet *pPacket) override;
};

#endif
