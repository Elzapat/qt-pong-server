#ifndef PONG_SERVER_H
#define PONG_SERVER_H

#include <QtCore>
#include <QtNetwork>

class PongServer : public QThread {
    Q_OBJECT

    public:
        explicit PongServer(QTcpSocket* p1_i, QTcpSocket* p2_i, QObject* parent = 0);
        ~PongServer();
        void run() override;

    private:
        QTcpSocket* p1;
        QTcpSocket* p2;
};

#endif // PONG_SERVER_H
