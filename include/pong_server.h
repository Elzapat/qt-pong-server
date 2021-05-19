#ifndef PONG_SERVER_H
#define PONG_SERVER_H

#include <QtCore>
#include <QtNetwork>
#include "player.h"
#include "ball.h"
#include "constants.h"

class PongServer : public QThread {
    Q_OBJECT

    public:
        explicit PongServer(QTcpSocket* p1_i, QTcpSocket* p2_i, QObject* parent = 0);
        ~PongServer();

    public slots:
        void update();
        void send_data(QString data);
        void player_scored(int player);

    signals:
        void server_ended();

    private:
        Player p1;
        Player p2;
        Ball ball;
        QTimer* update_timer;
        bool game_started;
};

#endif // PONG_SERVER_H
