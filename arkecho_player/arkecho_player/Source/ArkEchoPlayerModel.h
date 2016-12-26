#ifndef ARKECHOPLAYERMODEL_H
#define ARKECHOPLAYERMODEL_H

#include <QObject>

struct WsStringData;
class WebSocketServer;
class SecurityCode;

enum UpdateViewEnum
{
    TEST = 0
};

class ArkEchoPlayerModel : public QObject
{
    Q_OBJECT

public:
    ArkEchoPlayerModel(QObject *parent = 0);
    ~ArkEchoPlayerModel();

    void showQrDialog();

signals:
    void updateView(int);

private:
    WebSocketServer* webSocketServer_;
    SecurityCode* securityCode_;

private slots:
    void onTextMessageReceived(const WsStringData& message);
};

#endif // ARKECHOPLAYERMODEL_H
