#include "robot.h"
#include "packet/packet.h"
#include "packet/protobuf/msg.pb.h"
#include "packet/protobuf/protoId.pb.h"
bool Robot::Init() {
    if (!EpollClient::Init())
        return false;

    this->Connect("127.0.0.1", 2233);
    return true;
}
void Robot::RegisterMsgFunction() { EpollClient::RegisterMsgFunction(); }

bool Robot::Update() {
    bool rs = EpollClient::Update();
    if (IsConnected() && !_isSendMsg) {
        Proto::TsetMsg msg;
        msg.set_index(Proto::MsgId::SendData);
        msg.set_msg("nihc");

        size_t msgLen = msg.ByteSizeLong();
        Packet packet(msgLen);
        packet.SetMessgeId(Proto::MsgId::SendData);

        //序列化
        msg.SerializePartialToArray(packet.GetBuffer(), msgLen);
        packet.ChangeEndInedx(msgLen);

        std::cout << "发送消息:" << msg.msg().c_str() << std::endl;

        AddPacket(&packet);

        _isSendMsg = true;
        rs = true;
    }

    return rs;
}
