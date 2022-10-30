#include "httpRequestAccount.h"
#include "packet/packet.h"
#include "packet/protobuf/msg.pb.h"
#include "packet/protobuf/protoId.pb.h"
HttpRequestAccount::HttpRequestAccount(std::string username,
                                       std::string password)
    : HttpRequest(username), _password(password) {
    _curlRs = CRS_None;
    _method = HttpResquestMethod::HRM_Post;

    _url = "127.0.0.1/member_login_t.php";
    _params.append("account=")
        .append(_account)
        .append("&password=")
        .append(_password);
}

/* protected*/
void HttpRequestAccount::ProcessMsg(Json::Value value) {

    // 默认设置未知错误
    Proto::AccountCheckRs::ReturnCode code = Proto::AccountCheckRs::ARC_UNKONWN;

    // 解析json数据
    int httpcode = value["returncode"].asInt();

    switch (httpcode) {
    case 0:
        code = Proto::AccountCheckRs::ARC_OK;
        break;
    case 2:
        code = Proto::AccountCheckRs::ARC_NOT_FOUND_ACCOUNT;
        break;
    case 3:
        code = Proto::AccountCheckRs::ARC_PASSWORD_WRONG;
        break;
    case 4:
        code = Proto::AccountCheckRs::ARC_LOGGING;
        break;
    case 5:
        code = Proto::AccountCheckRs::ARC_TIMEOUT;
        break;
    }

    Proto::AccountCheckToHttpRs checkProto;
    checkProto.set_return_code(code);
    checkProto.set_account(_account);

    // 数据分发 account类处理
    auto pCheckPacket = new Packet(Proto::MsgId::MI_AccountCheckToHttpRs, 0);
    pCheckPacket->SerializeToBuffer(checkProto);
    DispatchPacket(pCheckPacket);
}

void HttpRequestAccount::Dispose() {}
