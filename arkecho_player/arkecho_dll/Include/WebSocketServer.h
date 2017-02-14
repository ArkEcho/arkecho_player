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
    // Verbindet Signal und Slot für neue Verbindung
	WebSocketServer(const QString &name, QObject *parent = 0);
	~WebSocketServer();

    bool checkIfConnectionIsOpen();
    void sendMessage(int messageType, QString& message);

    QString getWebSocketServerNetworkAdress();

signals:
    // Bei neuer WebSocket Connection ausgelöst
    void wsConnected();

    // Bei Trennung eines WebSocket Connection ausgelöst
    void wsDisconnected();

    // Werden bei neuer Nachricht ausgelöst und übergeben Nachricht + Sender als Pointer
	void newTextMessageReceived(QString);

private slots:
    // Ausgelöst bei neuer Verbindung eines WebSocket;verbindet mit restlichen Slots;fügt der Liste hinzu
	void newWSConnection();

    // Angeschlossen an Text/Binary Nachricht Signal der WebSockets;füllt Struct mit Sender WebSocket Pointer
	void onTextMessageReceived(const QString &message);

    // Bei Ende der Verbindung wird der Socket aus der Liste gelöscht
	void socketDisconnected();
};

#endif // WEBSOCKETSERVER_H
