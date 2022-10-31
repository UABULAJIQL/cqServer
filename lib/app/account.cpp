#include "account.h"

#include "entity/player.h"
#include "http/httpRequestAccount.h"
#include "packet/messageList.h"
#include "packet/packet.h"
#include "packet/protobuf/msg.pb.h"
#include "packet/protobuf/protoId.pb.h"
#include "thread/threadMgr.h"

/*public*/

bool Account::Init() { return true; }
void Account::RegisterMsgFunction() {
    auto *mcbf = new MessageCallBackFunction();

    // 接收客户端发送登录请求的协议
    mcbf->RegisterFunction(
        Proto::MsgId::C2L_AccountCheck,
        std::bind(&Account::HandleAccountCheck, this, std::placeholders::_1));

    // 服务端的login向http服务器发送请求后 http服务器返回的结果
    mcbf->RegisterFunction(Proto::MsgId::MI_AccountCheckToHttpRs,
                           std::bind(&Account::HandleAccountCheckToHttpRs, this,
                                     std::placeholders::_1));

    AttachCallBackHandler(mcbf);
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

    // 验证账号(HTTP) 添加http请求
    HttpRequestAccount *pHttp =
        new HttpRequestAccount(protoCheck.account(), protoCheck.password());
    // 将请求添加到线程中
    ThreadMgr::GetInstance()->AddObjToThread(pHttp);
}
void Account::HandleAccountCheckToHttpRs(Packet *pPacket) {

    auto proto = pPacket->ParseToProto<Proto::AccountCheckToHttpRs>();

    Player *pPlayer = _playerMgr.GetPlayer(proto.account());

    if (pPlayer == nullptr) {
        std::cout << "can't find player. account:" << proto.account().c_str()
                  << std::endl;
        return;
    }

    std::cout << "account check result account:" << proto.account()
              << " rs:" << proto.return_code()
              << " socket:" << pPlayer->GetSocket() << std::endl;

    Proto::AccountCheckRs protoResult;
    protoResult.set_return_code(proto.return_code());

    // 返回发送给客户端
    auto pResultPacket =
        new Packet(Proto::MsgId::C2L_AccountCheckRs, pPlayer->GetSocket());
    pResultPacket->SerializeToBuffer(protoResult);
    SendPacket(pResultPacket);
}
