#include <QCoreApplication>
#include <QTimer>
#include "../include/lobby_server.h"

int main(int argc, char** argv) {
    QCoreApplication app(argc, argv);

    LobbyServer lobby_server(&app);

    // QObject::connect(lobby_server, SIGNAL(finished()), &app, SLOT(quit()));
    // QTimer::singleShot(0, lobby_server, SLOT(run()));

    return app.exec();
}
