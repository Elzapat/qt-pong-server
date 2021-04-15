#include "../include/player.h"

Player::Player(QTcpSocket* sock) : socket(sock), x(0.f), y(0.f), score(0) {}

Player::~Player() {
    delete socket;
}
