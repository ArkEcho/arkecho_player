#ifndef ARKECHOPLAYERMODEL_H
#define ARKECHOPLAYERMODEL_H

#include <QObject>

class WebSocketServer;

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

signals:
    void updateView(int);

private:
    WebSocketServer* server_;
};

#endif // ARKECHOPLAYERMODEL_H
