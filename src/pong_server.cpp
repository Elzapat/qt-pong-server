#include "../include/pong_server.h"

PongServer::PongServer(QTcpSocket* p1_i, QTcpSocket* p2_i, QObject* parent) :
    QThread(parent), p1(p1_i), p2(p2_i) {

}

PongServer::~PongServer() {}

void PongServer::run() {

}
