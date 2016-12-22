#include "GlobalWebSocket.h"

#include <QMessageBox>

const QString WEBSOCKET_PATH = "ws://localhost:1234";

QWebSocket GlobalWebSocket::socket_;

QWebSocket* GlobalWebSocket::getGlobalWebSocket()
{
    return &socket_;
}

bool GlobalWebSocket::isConnected()
{
    return socket_.isValid();
}

void GlobalWebSocket::openGlobalWebSocket()
{
    socket_.open(WEBSOCKET_PATH);
}

void GlobalWebSocket::closeGlobalWebSocket()
{
    socket_.close();
}

void GlobalWebSocket::changeGlobalWebSocketAdress()
{
    closeGlobalWebSocket();
    //Global::setWebSocketServerPath();
    openGlobalWebSocket();
}

void GlobalWebSocket::showMessageBoxNoConnectionToServer()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Fehler!");
    msgBox.setText("Es konnte keine Verbindung zum Server hergestellt werden!");
    msgBox.exec();
}
