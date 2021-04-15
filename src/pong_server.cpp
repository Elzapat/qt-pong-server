#include "../include/pong_server.h"

const double FPS = 30.f;

PongServer::PongServer(QTcpSocket* p1_i, QTcpSocket* p2_i, QObject* parent) :
    QThread(parent), p1(p1_i), p2(p2_i), game_started(false), ball(FPS) {

    update_timer = new QTimer(this);
    this->connect(update_timer, &QTimer::timeout, this, [this]() {
        update_ball();
        send_data();
    });
    update_timer->start(1 / (1000 * FPS));
}

PongServer::~PongServer() {
    delete update_timer;
}

void PongServer::update_ball() {

}

void PongServer::send_data() {

}
