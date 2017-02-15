#ifndef WEBSOCKETSERVER_H
#define WEBSOCKETSERVER_H

#include "arkecho_dll_global.h"

#include <QWebSocketServer>
#include <QList>

class ARKECHO_DLL_EXPORT WebSocketServer : public QWebSocketServer
{
	Q_OBJECT

	QWebSocket* webSocket_;

public:
    // Verbindet Signal und Slot f�r neue Verbindung
	WebSocketServer(const QString &name, QObject *parent = 0);
	~WebSocketServer();

    bool checkIfConnectionIsOpen();
    void sendMessage(int messageType, QString& message);

    QString getWebSocketServerNetworkAdress();

signals:
    // Bei neuer WebSocket Connection ausgel�st
    void wsConnected();

    // Bei Trennung eines WebSocket Connection ausgel�st
    void wsDisconnected();

    // Werden bei neuer Nachricht ausgel�st und �bergeben Nachricht + Sender als Pointer
	void newTextMessageReceived(QString);

private slots:
    // Ausgel�st bei neuer Verbindung eines WebSocket;verbindet mit restlichen Slots;f�gt der Liste hinzu
	void newWSConnection();

    // Angeschlossen an Text/Binary Nachricht Signal der WebSockets;f�llt Struct mit Sender WebSocket Pointer
	void onTextMessageReceived(const QString &message);

    // Bei Ende der Verbindung wird der Socket aus der Liste gel�scht
	void socketDisconnected();
};

#endif // WEBSOCKETSERVER_H
