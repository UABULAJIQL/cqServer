#include "server.h"
#include "buffer/networkBuffer.h"
#include "network/connectObj.h"
#include "packet/packet.h"
#include "packet/protobuf/msg.pb.h"
bool Server::DataHandler() {
    for (auto bIter = _connects.begin(), eIter = _connects.end();
            bIter != eIter; ++bIter) {
        ConnectObj *pConnectObj = bIter->second;
        HandlerOne(pConnectObj);
    }
    if (_isShow)
        _isShow = false;
    return true;
}
void Server::HandlerOne(ConnectObj *pConnectObj) {
    //接收消息
    while (pConnectObj->HasRecvData()) {
        //确实可以吧GetRecvNetworkBuffer移除，然后直接来一个GetPacket!!!!!还有之前的数据插入扩容问题要别忘记了！！！！！！
        Packet *pPacket = pConnectObj->GetRecvNetworkBuffer()->GetPacket();
        if (pPacket == nullptr)
            break;
        //解析
        Proto::TsetMsg protoMsg;
        protoMsg.ParsePartialFromArray(pPacket->GetBuffer(),
                pPacket->UnavailableLength());

        std::cout << "接收到消息" << protoMsg.msg().c_str() << std::endl;

        pConnectObj->GetSendNetworkBuffer()->AddPacket(pPacket);
        // std::cout << pConnectObj->GetSendNetworkBuffer()->UnavailableLength()
        // << std::endl; std::cout << pConnectObj->HasSendData() << std::endl;

        ++_recvMsgCount;
        ++_sendMsgCount;
        _isShow = true;

        //是否pPacket
        pPacket->Dispose();
        if (pPacket != nullptr) {
            delete pPacket;
            pPacket = nullptr;
        }
    }
}
int Server::Accept() {
    int rs = EpollServer::Accept();
    _acceptCount += rs;
    _isShow = true;
    return rs;
}
