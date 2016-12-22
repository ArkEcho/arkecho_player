#ifndef GLOBALWEBSOCKET_H
#define GLOBALWEBSOCKET_H

#include <QWebSocket>

class GlobalWebSocket
{
private:
    GlobalWebSocket() {};
    ~GlobalWebSocket() {};

    static QWebSocket socket_;

public:
    // WebSocket als Pointer, SIGNAL SLOT
    static QWebSocket* getGlobalWebSocket();

    // True wenn der Socket Senden und Empfangen kann
    static bool isConnected();

    // Stellt die Verbindung des Socket her
    static void openGlobalWebSocket();

    // Schliesst die Verbindung
    static void closeGlobalWebSocket();

    // Ändert die Server Adresse, schliesst und öffnet die Verbindung
    static void changeGlobalWebSocketAdress();

    // Zeigt eine Message Box an, das keine Verbindung hergestellt wurde
    static void showMessageBoxNoConnectionToServer();
};

#endif //GLOBALWEBSOCKET_H
