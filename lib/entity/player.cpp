#include "player.h"
Player::Player(int socket, std::string account, std::string password) {
    _socket = socket;
    _account = account;
    _password = password;
}
std::string Player::GetAccout() const { return _account; }
int Player::GetSocket() { return _socket; }
