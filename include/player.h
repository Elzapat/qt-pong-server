#ifndef PLAYER_H
#define PLAYER_H

#include <QtNetwork>
#include "constants.h"

class Player : public QObject {
    Q_OBJECT

    public:
        Player(QTcpSocket* sock, int side);
        ~Player();
        void send_data(QString data);
        QTcpSocket* get_socket();
        qreal get_x() const;
        qreal get_y() const;
        int get_score() const;
        void set_score(int new_score);
        void scored();
        void update(double dt);

    public slots:
        void ready_read();

    private:
        QTcpSocket* socket;
        qreal x, y;
        int score;
        bool moving_up;
        bool moving_down;
};

#endif // PLAYER_H
