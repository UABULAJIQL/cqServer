#include "playerMgr.h"

#include "entity/player.h"

#include <iostream>
void PlayerMgr::AddPlayer(SOCKET socket, std::string account,
                          std::string password) {
    auto iter = _players.find(socket);
    if (iter != _players.end()) {
        std::cout << "角色已存在 添加失败" << std::endl;
        return;
    }

    _accounts.emplace(account, socket);
    _players.emplace(socket, new Player(socket, account, password));
}
void PlayerMgr::RemovePlayer(SOCKET socket) {
    auto iter = _players.find(socket);
    if (iter == _players.end()) {
        std::cout << "角色不存在 失败" << std::endl;
        return;
    }

    Player *pPlayer = iter->second;

    _players.erase(socket);
    _accounts.erase(pPlayer->GetAccout());

    delete pPlayer;
}
Player *PlayerMgr::GetPlayer(SOCKET socket) const {
    auto iter = _players.find(socket);
    if (iter == _players.end()) {
        return nullptr;
    }
    return iter->second;
}
Player *PlayerMgr::GetPlayer(std::string account) {
    auto iter = _accounts.find(account);
    if (iter == _accounts.end())
        return nullptr;

    SOCKET socket = iter->second;

    auto iterPlayer = _players.find(socket);
    if (iterPlayer == _players.end()) {

        _accounts.erase(account);

        return nullptr;
    }
    return iterPlayer->second;
}
void PlayerMgr::Dispose() {
    for (auto bIter = _players.begin(), eIter = _players.end(); bIter != eIter;
         ++bIter) {
        delete bIter->second;
    }
    _players.clear();

    _accounts.clear();
}
