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

    // 初始化 返回一个CURL指针 以为这一个会话的开始
    _pCurl = curl_easy_init();

    // 初始化可以批处理的CURLM指针
    _pMultiHandle = curl_multi_init();

    return true;
}

void HttpRequest::RegisterMsgFunction() {}

bool HttpRequest::Update() {

    // 执行状态相对对应的处理函数 并且改变状态
    switch (_state) {

    // 起始发送请求状态
    case HRS_Send:
        if (ProcessSend())
            _state = HRS_Process; // 目前这里是值设置重复了
                                  // 在ProcessSend()中已经修改了
        break;

    // 执行状态
    case HRS_Process:
        if (Process())
            _state = HRS_Over;
        break;

    // 结束状态
    case HRS_Over:
        ProcessOver();
        _state = HRS_NoActive;
        _active = false;

        break;
    // 超时状态超时处理
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

    // 设置要访问的URL地址
    curl_easy_setopt(_pCurl, CURLOPT_URL, _url.c_str());
    // 读取数据回调
    curl_easy_setopt(_pCurl, CURLOPT_READFUNCTION, NULL);
    // 写入数据回调
    curl_easy_setopt(_pCurl, CURLOPT_WRITEFUNCTION,
                     WriteFunction); // 设置server的返回的数据的接收方式

    // 写入数据的地址
    curl_easy_setopt(_pCurl, CURLOPT_WRITEDATA,
                     static_cast<void *>(&_responseBuffer));

    // 设置需要毫秒超时
    curl_easy_setopt(_pCurl, CURLOPT_NOSIGNAL, 1);

    // 在屏幕打印请求连接过程和返回http数据
    curl_easy_setopt(_pCurl, CURLOPT_VERBOSE, 0); // 打印调试信息
    curl_easy_setopt(_pCurl, CURLOPT_HEADER, 0);
    // 将响应头信息和相应体一起传给write_data
    // curl_easy_setopt( _pCurl, CURLOPT_CONNECTTIMEOUT, 3
    // ); curl_easy_setopt( _pCurl, CURLOPT_TIMEOUT, 3 );

    // 将单个CURL指针存入批处理CURL容器中
    curl_multi_add_handle(_pMultiHandle, _pCurl);

    // 判断和设置请求方式
    if (_method == HttpResquestMethod::HRM_Post) {
        curl_easy_setopt(_pCurl, CURLOPT_POST,
                         true); // 设置问非0表示本次操作为post
        curl_easy_setopt(_pCurl, CURLOPT_POSTFIELDS,
                         _params.c_str()); // post参数
    }

    // 切换状态为执行状态
    _state = HttpResquestState::HRS_Process;

    return true;
}

void HttpRequest::ProcessTimeout() const {

    // 账户http结果
    Proto::AccountCheckToHttpRs checkProto;
    checkProto.set_account(_account);
    // 设置为超时
    checkProto.set_return_code(Proto::AccountCheckRs::ARC_TIMEOUT);

    auto pCheckPacket = new Packet(Proto::MsgId::MI_AccountCheckToHttpRs, 0,
                                   checkProto.ByteSizeLong() + 1);

    // 序列化
    pCheckPacket->SerializeToBuffer(checkProto);

    // 广播分发
    DispatchPacket(pCheckPacket);
}

bool HttpRequest::ProcessOver() {
    // 从批处理中移除curl
    curl_multi_remove_handle(_pMultiHandle, _pCurl);
    // 清空释放CURL
    curl_easy_cleanup(_pCurl);
    // 清空释放CRULW
    curl_multi_cleanup(_pMultiHandle);
    return true;
}

bool HttpRequest::Process() {
    int running_handle_count;
    // 通知执行相应的动作
    CURLMcode curlMcode =
        curl_multi_perform(_pMultiHandle, &running_handle_count);
    //-1 执行失败
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

    // 请求超时
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

    // 成功获取消息
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

        // 将数据解析存入Json::Value中
        bool ok = jsonReader->parse(
            _responseBuffer.data(),
            _responseBuffer.data() + _responseBuffer.size(), &value, &errs);

        if (ok && errs.size() == 0) {
            // 成功调用处理Json消息的函数
            ProcessMsg(value);
        } else {
            std::cout << "json parse failed. " << _responseBuffer.c_str()
                      << std::endl;
        }

        delete jsonReader;
    }
}
