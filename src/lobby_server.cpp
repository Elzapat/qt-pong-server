#include "../include/lobby_server.h"

LobbyServer::LobbyServer(QObject* parent) : QObject(parent), current_id(1) {
    const QHostAddress SERVER_ADDRESS(QHostAddress::LocalHost);
    const quint16 PORT = 2929;

    socket = new QUdpSocket(this);
    socket->bind(SERVER_ADDRESS, PORT);

    // connect(socket, SIGNAL(readyRead()), this, SLOT(read_socket()));
}

LobbyServer::~LobbyServer() {
    delete socket;
}

void LobbyServer::run() {
    qDebug() << "lobby_server run";

    while (1) {
        if (socket->hasPendingDatagrams()) {
            this->read_socket();
        }
        // Remove lobbies that have been alive for more than 60 seconds
        lobbies.erase(std::remove_if(lobbies.begin(), lobbies.end(), [](Lobby l) {
            return l.alive_timer.elapsed() > 60000;
        }), lobbies.end());
    }

    emit finished();
}

void LobbyServer::read_socket() {
    QNetworkDatagram datagram = socket->receiveDatagram();
    process_datagram(datagram);
}

void LobbyServer::process_datagram(QNetworkDatagram datagram) {
    qDebug() << datagram.data();

    QList<QByteArray> args = datagram.data().split(';');
    QString command(args[0]);
    
    if (command == "lobbies")
        send_lobbies(datagram.senderAddress(), datagram.senderPort());
    else if (command == "create_lobby")
        create_lobby();
}

void LobbyServer::send_lobbies(QHostAddress address, quint16 port) {
    QString response = "lobbies;" + QString::number(lobbies.size()) + ';';

    for (Lobby lobby : lobbies) {
        response += QString::number(lobby.id) + ';';
        response += QString::number(lobby.player_count) + ';';
    }

    socket->writeDatagram(response.toLocal8Bit(), address, port);
}

void LobbyServer::create_lobby() {
    if (lobbies.size() >= MAX_LOBBIES) return;

    Lobby new_lobby;

    new_lobby.id = current_id;
    new_lobby.player_count = 0;
    new_lobby.alive_timer.start();

    lobbies.push_back(new_lobby);
    current_id = (current_id + 1) % MAX_LOBBIES;
}
