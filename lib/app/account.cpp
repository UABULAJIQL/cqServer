#include "account.h"

#include "http/httpRequestAccount.h"
#include "packet/packet.h"
#include "packet/protobuf/msg.pb.h"
#include "packet/protobuf/protoId.pb.h"
#include "thread/threadMgr.h"

/*public*/

bool Account::Init() { return true; }
void Account::RegisterMsgFunction() {
    RegisterFunction(
        Proto::MsgId::C2L_AccountCheck,
        std::bind(&Account::HandleAccountCheck, this, std::placeholders::_1));

    RegisterFunction(Proto::MsgId::MI_AccountCheckToHttpRs,
                     std::bind(&Account::HandleAccountCheckToHttpRs, this,
                               std::placeholders::_1));
}

bool Account::Update() { return true; }

void Account::Dispose() { _playerMgr.Dispose(); }

/*private*/

void Account::HandleAccountCheck(Packet *pPacket) {
    auto protoCheck = pPacket->ParseToProto<Proto::AccountCheck>();

    const auto socket = pPacket->GetSocket();

    auto pPlayer = _playerMgr.GetPlayer(protoCheck.account());

    if (pPacket != nullptr) {
        // 账号已经登入 返回客户端信息同时关闭网络
        Proto::AccountCheckRs protoResult;
        protoResult.set_return_code(Proto::AccountCheckRs::ARC_LOGGING);

        auto pRsPacket = new Packet(Proto::MsgId::C2L_AccountCheckRs, socket);
        pRsPacket->SerializeToBuffer(protoResult);
        SendPacket(pRsPacket);

        // 关闭网络
        const auto pPacketDis =
            new Packet(Proto::MsgId::MI_NetworkDisconnectToNet, socket);
        DispatchPacket(pPacketDis);

        return;
    }
    // 更新信息
    _playerMgr.AddPlayer(socket, protoCheck.account(), protoCheck.password());

    // 验证账号(HTTP)
    HttpRequestAccount *pHttp =
        new HttpRequestAccount(protoCheck.account(), protoCheck.password());

    ThreadMgr::GetInstance()->AddObjToThread(pHttp);
}
void Account::HandleAccountCheckToHttpRs(Packet *pPacket) {}
