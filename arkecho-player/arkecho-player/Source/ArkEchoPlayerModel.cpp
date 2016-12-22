#include "ArkEchoPlayerModel.h"
#include "WebSocketServer.h"

const QString SERVER_NAME = "ArkEcho Server";

ArkEchoPlayerModel::ArkEchoPlayerModel(QObject *parent)
    : QObject(parent)
{
    server_ = new WebSocketServer(SERVER_NAME);
}

ArkEchoPlayerModel::~ArkEchoPlayerModel()
{
    delete server_;
}
