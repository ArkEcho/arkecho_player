#ifndef ARKECHOPLAYERMODEL_H
#define ARKECHOPLAYERMODEL_H

#include <QObject>

struct WsStringData;
class WebSocketServer;
class MusicSongList;

enum UpdateViewEnum
{
    TEST = 0,
    UVE_WEBSOCKET_CONNECTED,
    UVE_WEBSOCKET_DISCONNECTED
};

class ArkEchoPlayerModel : public QObject
{
    Q_OBJECT

public:
    ArkEchoPlayerModel(QObject *parent = 0);
    ~ArkEchoPlayerModel();

    void showConnectQrDialog();
    void showConnectManualDialog();

signals:
    void updateView(int);

private:
    WebSocketServer* webSocketServer_;
    MusicSongList* musicSongList_;

    QString getWebServerAddress();

private slots:
    void onTextMessageReceived(const WsStringData& data);
    void onWSConnected();
    void onWSDisconnected();
};

#endif // ARKECHOPLAYERMODEL_H
