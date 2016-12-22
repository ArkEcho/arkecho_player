#ifndef WEBSOCKETSERVER_H
#define WEBSOCKETSERVER_H

#include <QWebSocketServer>
#include <QList>

struct WsStringData
{
    QWebSocket *socket_;
    QString message_;
};

class WebSocketServer : public QWebSocketServer
{
	Q_OBJECT

public:
    // Verbindet Signal und Slot f�r neue Verbindung
	WebSocketServer(const QString &name, QObject *parent = 0);
	~WebSocketServer();

	QWebSocket* getWebSocket();

    QString getWebSocketServerNetworkAdress();

signals:
    // Bei neuer WebSocket Connection ausgel�st
    void wsConnected();

    // Bei Trennung eines WebSocket Connection ausgel�st
    void wsDisconnected();

    // Werden bei neuer Nachricht ausgel�st und �bergeben Nachricht + Sender als Pointer
	void newTextMessageReceived(WsStringData);

private slots:
    // Ausgel�st bei neuer Verbindung eines WebSocket;verbindet mit restlichen Slots;f�gt der Liste hinzu
	void newWSConnection();

    // Angeschlossen an Text/Binary Nachricht Signal der WebSockets;f�llt Struct mit Sender WebSocket Pointer
	void onTextMessageReceived(const QString &message);

    // Bei Ende der Verbindung wird der Socket aus der Liste gel�scht
	void socketDisconnected();

private:
	QWebSocket* webSocket_;
};

#endif // WEBSOCKETSERVER_H
