#include "../include/lobby_server.h"

LobbyServer::LobbyServer(QObject* parent) : QObject(parent), current_id(1) {
    const QHostAddress SERVER_ADDRESS(QHostAddress::LocalHost);
    const quint16 PORT = 2929;

    server.listen(SERVER_ADDRESS, PORT);
    this->connect(&server, SIGNAL(newConnection()), this, SLOT(new_connection()));
}

LobbyServer::~LobbyServer() {}

void LobbyServer::new_connection() {
    qDebug() << "new_connection";
    QTcpSocket* client = server.nextPendingConnection();
    this->connect(client, SIGNAL(readyRead()), this, SLOT(ready_read()));
    this->connect(client, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            this, SLOT(state_changed(QAbstractSocket::SocketState)));
    clients.push_back(client);
}

void LobbyServer::ready_read() {
    QTcpSocket* sender = static_cast<QTcpSocket*>(QObject::sender());
    QByteArray data = sender->readAll();

    qDebug() << "new packet" << data;

    process_packet(sender, data);
}

void LobbyServer::state_changed(QAbstractSocket::SocketState state) {
    if (state == QAbstractSocket::UnconnectedState) {
        QTcpSocket* sender = static_cast<QTcpSocket*>(QObject::sender());
        clients.removeOne(sender);
        for (Lobby& lobby : lobbies) {
            if (lobby.p1 == sender) {
                lobby.p1 = nullptr;
                lobby.player_count--;
            } else if (lobby.p2 == sender) {
                lobby.p2 = nullptr;
                lobby.player_count--;
            }
        }
    }
}

void LobbyServer::process_packet(QTcpSocket* sender, QByteArray data) {
    QList<QByteArray> args_bytes = data.split(';');
    QStringList args;
    for (auto arg_bytes : args_bytes)
        args.append(QString::fromLocal8Bit(arg_bytes));

    if (args[0] == "lobbies")
        send_lobbies(sender);
    else if (args[0] == "create_lobby")
        create_lobby();
    else if (args[0] == "join_lobby")
        join_lobby(sender, args[1].toInt());
    else if (args[0] == "leave_lobby")
        leave_lobby(sender, args[1].toInt());
}

void LobbyServer::send_lobbies(QTcpSocket* socket) {
    QString response = "lobbies;" + QString::number(lobbies.size()) + ';';

    for (Lobby& lobby : lobbies) {
        response += QString::number(lobby.id) + ';';
        response += QString::number(lobby.player_count) + ';';
    }

    socket->write(response.toLocal8Bit());
}

void LobbyServer::create_lobby() {
    if (lobbies.size() >= MAX_LOBBIES) return;

    Lobby new_lobby;

    new_lobby.id = current_id;
    new_lobby.player_count = 0;
    new_lobby.alive_timer = new QTimer;
    // After 60 seconds, we delete the lobby
    new_lobby.alive_timer->start(60000);

    int id = current_id;
    this->connect(new_lobby.alive_timer, &QTimer::timeout, this, [this, id]() {
        kill_lobby(id);
    });

    lobbies.insert(new_lobby.id, new_lobby);

    current_id = (current_id + 1) % MAX_LOBBIES;
}

void LobbyServer::join_lobby(QTcpSocket* sender, int id) {
    if (!lobbies.contains(id)) return;

    sender->write(("join_lobby;" + QString::number(id) + ';').toLocal8Bit());

    lobbies[id].player_count++;
    if (lobbies[id].p1 == nullptr)
        lobbies[id].p1 = sender;
    else
        lobbies[id].p2 = sender;

    if (lobbies[id].player_count == 2)
        start_game(id);
}

void LobbyServer::leave_lobby(QTcpSocket* sender, int id) {
    if (!lobbies.contains(id)) return;

    sender->write(("leave_lobby;" + QString::number(id) + ';').toLocal8Bit());

    lobbies[id].player_count--;
    if (lobbies[id].p1 == sender) {
        delete lobbies[id].p1;
        lobbies[id].p1 = nullptr;
    } else if (lobbies[id].p2 == sender) {
        delete lobbies[id].p2;
        lobbies[id].p2 = nullptr;
    }
}

void LobbyServer::kill_lobby(int id) {
    delete lobbies[id].alive_timer;
    lobbies.remove(id); 
    // Tell all the clients a lobby has died
    for (QTcpSocket* client : clients)
        client->write(("lobby_died;" + QString::number(id) + ';').toLocal8Bit());
}

void LobbyServer::start_game(int id) {
    clients.removeOne(lobbies[id].p1);
    clients.removeOne(lobbies[id].p1);
    kill_lobby(id);

    PongServer* server = new PongServer(lobbies[id].p1, lobbies[id].p2, this);
    server->start();
    servers.push_back(server);
}
