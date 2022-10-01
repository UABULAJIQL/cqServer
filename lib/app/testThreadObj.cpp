#include "testThreadObj.h"
#include "packet/packet.h"
#include "packet/protobuf/msg.pb.h"
#include "packet/protobuf/protoId.pb.h"
bool TestThreadObj::Init() { return true; }
void TestThreadObj::RegisterMsgFunction() {
    //在这边注册一个协议
    RegisterFunction(
            Proto::MsgId::SendData,
            std::bind(&TestThreadObj::HandleMsg, this, std::placeholders::_1));
}

void TestThreadObj::HandleMsg(Packet *pPacket) {

    // 解析
    Proto::TsetMsg protoMsg;
    protoMsg.ParsePartialFromArray(pPacket->GetBuffer(),
            pPacket->UnavailableLength());
    std::cout << " 接收到消息:" << protoMsg.msg().c_str() << std::endl;

    // pPacket->Dispose();
    // if (pPacket != nullptr) {
    //     delete pPacket;
    //     pPacket = nullptr;
    // }
}
bool TestThreadObj::Update() { 

    return true; }
void TestThreadObj::Dispose() {}
