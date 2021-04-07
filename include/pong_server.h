#ifndef PONG_SERVER_H
#define PONG_SERVER_H

#include <QtCore>
#include <QtNetwork>

class PongServer : public QThread {
    Q_OBJECT

    public:
        explicit PongServer(QObject* parent = 0);
        ~PongServer();
        void run() override;
};

#endif // PONG_SERVER_H
