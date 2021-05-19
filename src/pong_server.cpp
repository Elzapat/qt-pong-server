#include "../include/pong_server.h"

PongServer::PongServer(QTcpSocket* p1_i, QTcpSocket* p2_i, QObject* parent) :
    QThread(parent), p1(p1_i, 1), p2(p2_i, 2), game_started(false) {

    update_timer = new QTimer(this);
    this->connect(update_timer, SIGNAL(timeout()), this, SLOT(update()));
    update_timer->start(1.f / FPS * 1000.f);

    // Wait a second and then tell the players which side they're on
    QTimer::singleShot(1000, [this] {
        p1.send_data("side;1");
        p2.send_data("side;2");
        // Two seconds after that start the game
        QTimer::singleShot(2000, [this] { ball.launch(); });
    });

    this->connect(p1.get_socket(), &QAbstractSocket::disconnected, this, [this] {
        emit server_ended();
        send_data("end;");
        p2.get_socket()->disconnectFromHost();
    });
    this->connect(p2.get_socket(), &QAbstractSocket::disconnected, this, [this] {
        emit server_ended();
        send_data("end;");
        p1.get_socket()->disconnectFromHost();
    });
}

PongServer::~PongServer() {
    // p1.get_socket()->disconnectFromHost();
    // p2.get_socket()->disconnectFromHost();
    // qDebug() << "pong server closed";
    // delete update_timer;
}

void PongServer::update() {
    ball.update();
    int player = ball.collision(p1, p2);
    if (player != 0)
        player_scored(player);

    p1.update(1.f / FPS);
    p2.update(1.f / FPS);

    send_data("ball;" + QString::number(ball.get_x()) + ';' + QString::number(ball.get_y()));
    send_data("move;1;" + QString::number(p1.get_x()) + ';' + QString::number(p1.get_y()));
    send_data("move;2;" + QString::number(p2.get_x()) + ';' + QString::number(p2.get_y()));
}

void PongServer::send_data(QString data) {
    p1.send_data(data);
    p2.send_data(data);
}

void PongServer::player_scored(int player) {
    send_data("score;" + QString::number(player));
    ball.point_scored(player);
    if (player == 1)
        p1.scored();
    else
        p2.scored();

    if (p1.get_score() >= SCORE_TO_WIN)
        send_data("won;1");
    else if (p2.get_score() >= SCORE_TO_WIN)
        send_data("won;2");
    else {
        // No player has won yet
        QTimer::singleShot(2000, [this] { ball.launch(); });
        return;
    }

    // One player has won, we wait a bit longer before starting the game again
    p1.set_score(0);
    p2.set_score(0);
    QTimer::singleShot(5000, [this] { ball.launch(); });
}
