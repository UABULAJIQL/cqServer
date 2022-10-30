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

    Proto::AccountCheckRs::ReturnCode code = Proto::AccountCheckRs::ARC_UNKONWN;

    // 解析json数据
    int httpcode = value["returncode"].asInt();
    if (httpcode == 0)
        code = Proto::AccountCheckRs::ARC_OK;
    else if (httpcode == 2)
        code = Proto::AccountCheckRs::ARC_NOT_FOUND_ACCOUNT;
    else if (httpcode == 3)
        code = Proto::AccountCheckRs::ARC_PASSWORD_WRONG;

    Proto::AccountCheckToHttpRs checkProto;
    checkProto.set_return_code(code);
    checkProto.set_account(_account);

    auto pCheckPacket = new Packet(Proto::MsgId::MI_AccountCheckToHttpRs, 0);
    pCheckPacket->SerializeToBuffer(checkProto);
    DispatchPacket(pCheckPacket);
}

void HttpRequestAccount::Dispose() {}
