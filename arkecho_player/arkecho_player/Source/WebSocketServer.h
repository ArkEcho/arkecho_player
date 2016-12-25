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
    // Verbindet Signal und Slot für neue Verbindung
	WebSocketServer(const QString &name, QObject *parent = 0);
	~WebSocketServer();

	QWebSocket* getWebSocket();

    QString getWebSocketServerNetworkAdress();

signals:
    // Bei neuer WebSocket Connection ausgelöst
    void wsConnected();

    // Bei Trennung eines WebSocket Connection ausgelöst
    void wsDisconnected();

    // Werden bei neuer Nachricht ausgelöst und übergeben Nachricht + Sender als Pointer
	void newTextMessageReceived(WsStringData);

private slots:
    // Ausgelöst bei neuer Verbindung eines WebSocket;verbindet mit restlichen Slots;fügt der Liste hinzu
	void newWSConnection();

    // Angeschlossen an Text/Binary Nachricht Signal der WebSockets;füllt Struct mit Sender WebSocket Pointer
	void onTextMessageReceived(const QString &message);

    // Bei Ende der Verbindung wird der Socket aus der Liste gelöscht
	void socketDisconnected();

private:
	QWebSocket* webSocket_;
};

#endif // WEBSOCKETSERVER_H
