#ifndef _HTTPREQUESTACCOUNT_H
#define _HTTPREQUESTACCOUNT_H

#include "http/httpRequest.h"

#include <string>

class HttpRequestAccount : public HttpRequest {
  public:
    HttpRequestAccount(std::string username, std::string password);
    void Dispose() override;

  protected:
    // 从http服务器获取到数据后的处理函数
    virtual void ProcessMsg(Json::Value value) override;

  private:
    std::string _password;
};

#endif
