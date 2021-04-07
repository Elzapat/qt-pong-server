#ifndef LOBBY_SERVER_H
#define LOBBY_SERVER_H

#include <QtCore>
#include <QtNetwork>
#include <QDebug>
#include <QDateTime>
#include <algorithm>

class LobbyServer : public QObject {
    Q_OBJECT

    public:
        explicit LobbyServer(QObject* parent = 0);
        ~LobbyServer();

    public slots:
        void run();
        void read_socket();

    signals:
        void finished();

    private:
        struct Lobby {
            int id;
            quint8 player_count;
            QElapsedTimer alive_timer;
        };
        const int MAX_LOBBIES = 5;
        int current_id;
        void process_datagram(QNetworkDatagram datagram);
        void send_lobbies(QHostAddress address, quint16 port);
        void create_lobby();
        QUdpSocket* socket;
        QVector<Lobby> lobbies;

};

#endif // LOBBY_SERVER_H
