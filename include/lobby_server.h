#ifndef LOBBY_SERVER_H
#define LOBBY_SERVER_H

#include <QtCore>
#include <QtNetwork>
#include <QDebug>
#include <algorithm>
#include "pong_server.h"

class LobbyServer : public QObject {
    Q_OBJECT

    public:
        explicit LobbyServer(QObject* parent = 0);
        ~LobbyServer();

    public slots:
        void new_connection();
        void ready_read();
        void state_changed(QAbstractSocket::SocketState state);
        void pong_server_ended();

    private:
        struct Lobby {
            int id;
            quint8 player_count;
            QTimer* alive_timer = nullptr;
            QTcpSocket* p1 = nullptr;
            QTcpSocket* p2 = nullptr;
            // bool operator==(const Lobby& rhs) { return this->id == rhs.id; }
        };
        const int MAX_LOBBIES = 5;
        int current_id;
        void process_packet(QTcpSocket* sender, QByteArray data);
        void send_lobbies(QTcpSocket* socket);
        void create_lobby();
        void join_lobby(QTcpSocket* sender, int id);
        void leave_lobby(QTcpSocket* sender, int id);
        void kill_lobby(int id);
        void start_game(int id);
        QTcpServer server;
        QMap<int, Lobby> lobbies;
        QVector<QTcpSocket*> clients;
        QVector<PongServer*> servers;
};

#endif // LOBBY_SERVER_H
