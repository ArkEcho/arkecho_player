#include "ArkEchoPlayerModel.h"
#include "WebSocketServer.h"

const QString SERVER_NAME = "ArkEcho Server";

ArkEchoPlayerModel::ArkEchoPlayerModel(QObject *parent)
    : QObject(parent)
{
    webSocketServer_ = new WebSocketServer(SERVER_NAME);
    if (webSocketServer_->listen(QHostAddress::Any, 1000)) // Port festlegen
    {
        //QString s = webSocketServer_->getWebSocketServerNetworkAdress();
        /*connect(webSocketServer_, SIGNAL(newTextMessageReceived(WsStringData)), this, SLOT(onTextMessageReceived(WsStringData)));
        connect(webSocketServer_, SIGNAL(wsConnected()), this, SLOT(onWSConnected()));
        connect(webSocketServer_, SIGNAL(wsDisconnected()), this, SLOT(onWSDisconnected()));*/
    }
}

ArkEchoPlayerModel::~ArkEchoPlayerModel()
{
    webSocketServer_->close();

    delete webSocketServer_;
}
