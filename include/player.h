#ifndef PLAYER_H
#define PLAYER_H

#include <QtNetwork>

class Player {
    public:
        Player(QTcpSocket* sock);
        ~Player();

    private:
        QTcpSocket* socket;
        qreal x, y;
        int score;
};

#endif // PLAYER_H
