#include "WebSocketServer.h"

#include <QWebSocket>
#include <QNetWorkInterface>

WebSocketServer::WebSocketServer(const QString &name, QObject *parent)
    : QWebSocketServer(name, QWebSocketServer::NonSecureMode, parent)
    , webSocket_(0)
{
	connect(this, SIGNAL(newConnection()), this, SLOT(newWSConnection()));
}

void WebSocketServer::newWSConnection()
{
    if (webSocket_) return; // Gibt es bereits eine Verbindung wird abgebrochen
    webSocket_ = nextPendingConnection();

	connect(webSocket_, SIGNAL(textMessageReceived(QString)), this, SLOT(onTextMessageReceived(QString)));
	connect(webSocket_, SIGNAL(disconnected()), this, SLOT(socketDisconnected()));

	emit wsConnected();
}

void WebSocketServer::onTextMessageReceived(const QString &message)
{
    WsStringData data;

    data.message_ = message;
    data.socket_ = qobject_cast<QWebSocket*>(sender());

    emit newTextMessageReceived(data); // Auslösen des Signals des WebSocketServer
}

void WebSocketServer::socketDisconnected()
{
    delete webSocket_;
    emit wsDisconnected();
}

WebSocketServer::~WebSocketServer()
{
    delete webSocket_;
}

QWebSocket* WebSocketServer::getWebSocket()
{
	return webSocket_;
}

QString WebSocketServer::getWebSocketServerNetworkAdress()
{
    QString networkAddress = "";
    foreach(const QHostAddress &address, QNetworkInterface::allAddresses())
    {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost))
        {
            networkAddress = address.toString();
            if (networkAddress.startsWith("192.168") || networkAddress.startsWith("141.7")) return networkAddress;
        }
    }
    return networkAddress;
}
