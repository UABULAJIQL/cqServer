// 玩家实体类
#ifndef _PLAYER_H
#define _PLAYER_H

#include "tools/socketObj.h"
#include <string>

class Player : public ISocketObject {
  public:
    Player(int socket, std::string account, std::string password);
    std::string GetAccout() const;
    int GetSocket() override;

  private:
    std::string _account;
    std::string _password;
    int _socket;
};

#endif
