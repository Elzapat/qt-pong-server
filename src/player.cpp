#include "../include/player.h"

Player::Player(QTcpSocket* sock, int side) : socket(sock),
         x(side == 1 ? -BOARD_W / 2 + PADDLE_SPACING : BOARD_W / 2 - PADDLE_SPACING),
         y(0.f), score(0), moving_up(false), moving_down(false) {
    this->connect(socket, SIGNAL(readyRead()), this, SLOT(ready_read()));
}

Player::~Player() {
    // delete socket;
}

void Player::send_data(QString data) {
    socket->write((data + '%').toLocal8Bit());
}

QTcpSocket* Player::get_socket() {
    return socket;
}

void Player::ready_read() {
    QByteArray data = socket->readAll();

    // qDebug() << "player received packet: " << data;

    QStringList packets;
    for (QByteArray packet_bytes : data.split('%'))
        packets.append(QString::fromLocal8Bit(packet_bytes));

    for (QString packet : packets) {
        QStringList args = packet.split(';');

        if (args[0] == "move") {
            if (args[1] == "up")
                moving_up = (bool)args[2].toInt();
            else if (args[1] == "down")
                moving_down = (bool)args[2].toInt();
        }
    }
}

qreal Player::get_x() const {
    return x;
}

qreal Player::get_y() const {
    return y;
}

void Player::update(double dt) {
    if (moving_up)
        y -= PADDLE_SPEED * dt;
    
    if (moving_down)
        y += PADDLE_SPEED * dt;

    if (y <= -BOARD_H / 2)
        y = -BOARD_H / 2;

    if (y + PADDLE_H >= BOARD_H / 2)
        y = BOARD_H / 2 - PADDLE_H;
}

void Player::scored() {
    score++;
}

int Player::get_score() const {
    return score;
}

void Player::set_score(int new_score) {
    score = new_score;
}
