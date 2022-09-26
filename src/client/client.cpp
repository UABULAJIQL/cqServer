#include "client.h"
#include "packet/packet.h"
#include "packet/protobuf/msg.pb.h"
#include "packet/protobuf/protoId.pb.h"

#include <iostream>
#include <random>
#include <sstream>
#include <thread>

Client::Client(int msgCount, std::thread::id threadId) {
    _msgCount = msgCount;

    std::stringstream strStream;
    strStream << threadId;
    std::string idstr = strStream.str();
    std::seed_seq seed1(idstr.begin(), idstr.end());
    std::minstd_rand0 generator(seed1);

    _pRandomEngine = new std::default_random_engine(generator());
}
bool Client::Connect(std::string ip, int port) {
    if (EpollClient::Connect(ip, port)) {
        _lastMsg = "";
        return true;
    }
    return false;
}
void Client::DataHandler() {
    if (_isCompleted) {
        return;
    }
    if (!IsConnected()) {
        return;
    }

    //当前数与目标数比较
    if (_index < _msgCount) {
        if (_lastMsg.empty()) {
            //随机字符串
            _lastMsg = GenRandowm();

            Proto::TsetMsg protoMsg;
            protoMsg.set_index(_index);
            protoMsg.set_msg(_lastMsg.c_str());

            auto msgLen = protoMsg.ByteSizeLong();

            Packet packet(msgLen + 1);
            packet.SetMessgeId(Proto::MsgId::SendData);

            //序列化
            protoMsg.SerializePartialToArray(packet.GetBuffer(), msgLen);
            packet.ChangeEndInedx(msgLen);

            std::cout << _index << "发送消息:" << protoMsg.msg().c_str() << std::endl;
                // << " protobuf长度:" << msgLen
                // << " packet使用长度:" << packet.UnavailableLength()
                // << std::endl;

            AddPacket(&packet);

            // std::cout << "packet使用长度2:" << packet.UnavailableLength() << std::endl;
            

            /* //暂时先不接收消息
            _lastMsg = "";
            ++_index; */
        } else {
            if (GetConnectObj()->HasRecvData()) {

                Packet *pPacket = GetPacket();
                if (pPacket != nullptr) {
                    //解析
                    // ParsePartialFromArray
                    Proto::TsetMsg protoMsg;
                    protoMsg.ParsePartialFromArray(
                            pPacket->GetBuffer(), pPacket->UnavailableLength());

                    std::cout << "接收到消息:" << protoMsg.msg().c_str()
                        << std::endl;

                    _lastMsg = "";
                    ++_index;

                    pPacket->Dispose();
                    delete pPacket;
                    pPacket = nullptr;
                }
            }
        }

    } else {
        //发送完成
        _isCompleted = true;
    }
}
bool Client::IsCompleted() const { return _isCompleted; }
std::string Client::GenRandowm() {
    // std::cout << "随机消息" << std::endl;

    std::uniform_int_distribution<int> distribution{'a', 'z'};
    std::uniform_int_distribution<int> disLength(10, 20);
    int length = disLength(*_pRandomEngine);

    std::string rand_str(length, '\0');
    for (auto &one : rand_str) {
        one = distribution(*_pRandomEngine);
    }

    return rand_str;
}
void Client::Dispose() {
    EpollClient::Dispose();
    if (_pRandomEngine != nullptr)
        delete _pRandomEngine;
    _pRandomEngine = nullptr;
}
