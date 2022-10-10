#include "httpRequest.h"

#include "packet/packet.h"
#include "packet/protobuf/msg.pb.h"
#include "packet/protobuf/protoId.pb.h"

size_t WriteFunction(void *buffer, size_t size, size_t nmemb, void *lpVoid) {
    std::string *str = static_cast<std::string *>(lpVoid);
    if (nullptr == str || nullptr == buffer) {
        return -1;
    }

    char *pData = static_cast<char *>(buffer);
    str->append(pData, size * nmemb);

    return size * nmemb;
}

HttpRequest::HttpRequest(std::string account) : _account(account) {}

bool HttpRequest::Init() {

    _pCurl = curl_easy_init();
    _pMultiHandle = curl_multi_init();

    return true;
}

void HttpRequest::RegisterMsgFunction() {}
bool HttpRequest::Update() {

    switch (_state) {

    case HRS_Send:
        if (ProcessSend())
            _state = HRS_Process;
        break;

    case HRS_Process:
        if (Process())
            _state = HRS_Over;

        break;
    case HRS_Over:
        ProcessOver();
        _state = HRS_NoActive;
        _active = false;

        break;
    case HRS_Timeout:
        ProcessTimeout();
        _state = HRS_NoActive;
        _active = false;

        break;
    default:
        break;
    }

    return true;
}
bool HttpRequest::ProcessSend() {
    curl_easy_setopt(_pCurl, CURLOPT_URL, _url.c_str());
    curl_easy_setopt(_pCurl, CURLOPT_READFUNCTION, NULL);
    curl_easy_setopt(_pCurl, CURLOPT_WRITEFUNCTION,
                     WriteFunction); // 设置server的返回的数据的接收方式
    curl_easy_setopt(_pCurl, CURLOPT_WRITEDATA,
                     static_cast<void *>(&_responseBuffer));

    curl_easy_setopt(_pCurl, CURLOPT_NOSIGNAL, 1);
    curl_easy_setopt(_pCurl, CURLOPT_VERBOSE, 0); // 打印调试信息
    curl_easy_setopt(_pCurl, CURLOPT_HEADER,
                     0); // 将响应头信息和相应体一起传给write_data
                         // curl_easy_setopt( _pCurl, CURLOPT_CONNECTTIMEOUT, 3
                         // ); curl_easy_setopt( _pCurl, CURLOPT_TIMEOUT, 3 );

    curl_multi_add_handle(_pMultiHandle, _pCurl);

    if (_method == HttpResquestMethod::HRM_Post) {
        curl_easy_setopt(_pCurl, CURLOPT_POST,
                         true); // 设置问非0表示本次操作为post
        curl_easy_setopt(_pCurl, CURLOPT_POSTFIELDS,
                         _params.c_str()); // post参数
    }

    _state = HttpResquestState::HRS_Process;
    return true;
}
void HttpRequest::ProcessTimeout() const {

    Proto::AccountCheckToHttpRs checkProto;
    checkProto.set_account(_account);
    checkProto.set_return_code(Proto::AccountCheckRs::ARC_TIMEOUT);

    auto pCheckPacket = new Packet(Proto::MsgId::MI_AccountCheckToHttpRs, 0);

    pCheckPacket->SerializeToBuffer(checkProto);

    DispatchPacket(pCheckPacket);
}

bool HttpRequest::ProcessOver() {
    curl_multi_remove_handle(_pMultiHandle, _pCurl);
    curl_easy_cleanup(_pCurl);
    curl_multi_cleanup(_pMultiHandle);
    return true;
}

bool HttpRequest::Process() {
    int running_handle_count;
    CURLMcode curlMcode =
        curl_multi_perform(_pMultiHandle, &running_handle_count);
    if (CURLM_CALL_MULTI_PERFORM == curlMcode)
        return false;

    // 出错，结束
    if (curlMcode != CURLMcode::CURLM_OK) {
        _curlRs = CRS_CURLMError;
        _state = HRS_Timeout;
        return true;
    }

    // 处理完了，不再处理
    if (running_handle_count == 0) {
        ProcessMsg();
        return true;
    }

    CURLMRS rs = curl_multi_select(_pMultiHandle);
    if (rs != CRS_OK && rs != CRS_CURLM_CALL_MULTI_PERFORM) {
        _curlRs = rs;
        _state = HRS_Timeout;
        return true;
    }

    // 下一Frame继续执行
    return false;
}

void HttpRequest::ProcessMsg() {
    int msgs_left;
    CURLMsg *msg;
    msg = curl_multi_info_read(_pMultiHandle, &msgs_left);
    if (CURLMSG_DONE == msg->msg) {
        // if ( msg->easy_handle != _pCurl )
        //     std::cerr << "curl not found" << std::endl;

        // INFO_MSG( fmt::format( "curl completed with status:{}\n",
        // msg->data.result ) ); std::cout << "curl context:" <<
        // _responseBuffer.c_str() << std::endl;

        Json::CharReaderBuilder readerBuilder;
        Json::CharReader *jsonReader = readerBuilder.newCharReader();

        Json::Value value;
        JSONCPP_STRING errs;

        bool ok = jsonReader->parse(
            _responseBuffer.data(),
            _responseBuffer.data() + _responseBuffer.size(), &value, &errs);
        if (ok && errs.size() == 0) {
            ProcessMsg(value);
        } else {
            std::cout << "json parse failed. " << _responseBuffer.c_str()
                      << std::endl;
        }

        delete jsonReader;
    }
}
