#ifndef _PLAYERMGR_H
#define _PLAYERMGR_H

#include "tools/disposable.h"
#include <map>
#include <string>

using SOCKET = int;

class Player;

class PlayerMgr : public IDisposable {
  public:
    void AddPlayer(SOCKET socket, std::string account, std::string password);
    void RemovePlayer(SOCKET socket);

    Player *GetPlayer(SOCKET socket) const;
    Player *GetPlayer(std::string account);

    void Dispose() override;

  private:
    std::map<SOCKET, Player *> _players;
    std::map<std::string, SOCKET> _accounts;
};

#endif
