#ifndef _HTTPREQUEST_H
#define _HTTPREQUEST_H

#include "thread/threadObject.h"

#include <curl/curl.h>
#include <jsoncpp/json/json.h>
#include <string>

// 当前请求状态
enum HttpResquestState {
    HRS_Send,     // 发送状态
    HRS_Process,  // 执行等待状态
    HRS_Over,     // 完成
    HRS_NoActive, // 完成后的非激活状态，等待线程删除
    HRS_Timeout,  // 请求超时
};

enum HttpResquestMethod {
    HRM_Post,
    HRM_Get,
};

// 请求的返回结果
enum CURLMRS {
    CRS_None,
    CRS_OK,
    CRS_CURLM_CALL_MULTI_PERFORM, // call curl_multi_perform
    CRS_TIMEOUT,
    CRS_CURLMError, // curlm 调用出错
    CRS_SelectError,
};

class HttpRequest : public ThreadObject {

  protected:
    // http 请求方式 默认为get
    HttpResquestMethod _method{HRM_Get};

    // CURL指针
    CURL *_pCurl{nullptr};
    // 批处理CURL的指针容器
    CURLM *_pMultiHandle{nullptr};

    CURLMRS _curlRs;

    std::string _url, _params;

    std::string _responseBuffer;

    HttpResquestState _state{HRS_Send};

    std::string _account;

  public:
    // 构造函数 请求需要账户
    explicit HttpRequest(std::string account);
    // 初始化 主要是curl库中的初始化
    bool Init() override;
    void RegisterMsgFunction() override;

    // 帧中 判断状态 后给予对应的答复状态
    bool Update() override;

  protected:
    // 发送请求
    bool ProcessSend();
    // 发送超时处理
    void ProcessTimeout() const;

    // 请求结束
    virtual bool ProcessOver();

    // 执行状态
    virtual bool Process();
    // 执行后成功后获取消息
    virtual void ProcessMsg();

    // 服务端成功从http去获取到数据后如何处理数据
    virtual void ProcessMsg(Json::Value value) = 0;
};

inline CURLMRS curl_multi_select(CURLM *curl_m) {

    struct timeval timeout_tv;
    fd_set fd_read;
    fd_set fd_write;
    fd_set fd_except;
    int max_fd = -1;

    // 注意这里一定要清空fdset,curl_multi_fdset不会执行fdset的清空操作  //
    FD_ZERO(&fd_read);
    FD_ZERO(&fd_write);
    FD_ZERO(&fd_except);

    // 设置select超时时间
    timeout_tv.tv_sec = 1;
    timeout_tv.tv_usec = 0;

    // 获取multi curl需要监听的文件描述符集合 fd_set //
    curl_multi_fdset(curl_m, &fd_read, &fd_write, &fd_except, &max_fd);

    /**
     * When max_fd returns with -1,
     * you need to wait a while and then proceed and call curl_multi_perform
     * anyway. How long to wait? I would suggest 100 milliseconds at least, but
     * you may want to test it out in your own particular conditions to find a
     * suitable value.
     */
    if (-1 == max_fd) {
        return CURLMRS::CRS_CURLM_CALL_MULTI_PERFORM;
    }

    /**
     * 执行监听，当文件描述符状态发生改变的时候返回
     * 返回0，程序调用curl_multi_perform通知curl执行相应操作
     * 返回-1，表示select错误
     * 注意：即使select超时也需要返回0，具体可以去官网看文档说明
     */
    int ret_code =
        ::select(max_fd + 1, &fd_read, &fd_write, &fd_except, &timeout_tv);
    switch (ret_code) {
    case -1:
        /* select error */
        return CRS_SelectError;
    case 0:
        /* select timeout */
        return CRS_TIMEOUT;
    default:
        /* one or more of curl's file descriptors say there's data to read or
         * write*/
        return CRS_OK;
    }
}

#endif
