#include "ArkEchoPlayerModel.h"
#include "ArkEchoQr.h"
#include "WebSocketServer.h"
#include "SecurityCode.h"

#include <QJsonObject>
#include <QJsonDocument>

const QString SERVER_NAME = "ArkEcho Server";
const QString JSON_ADDRESS = "Address";
const QString JSON_SECURITY_CODE = "Security_Code";

ArkEchoPlayerModel::ArkEchoPlayerModel(QObject *parent)
    : QObject(parent)
{
    securityCode_ = new SecurityCode();

    webSocketServer_ = new WebSocketServer(SERVER_NAME);
    if (webSocketServer_->listen(QHostAddress::Any, 1000)) // Port festlegen
    {
        //QString s = webSocketServer_->getWebSocketServerNetworkAdress();
        connect(webSocketServer_, SIGNAL(newTextMessageReceived(WsStringData)), this, SLOT(onTextMessageReceived(WsStringData)));
        /*connect(webSocketServer_, SIGNAL(wsConnected()), this, SLOT(onWSConnected()));
        connect(webSocketServer_, SIGNAL(wsDisconnected()), this, SLOT(onWSDisconnected()));*/
    }
}

ArkEchoPlayerModel::~ArkEchoPlayerModel()
{
    if(webSocketServer_) webSocketServer_->close();

    delete webSocketServer_;
    delete securityCode_;
}

void ArkEchoPlayerModel::showQrDialog()
{
    if (!webSocketServer_ || !securityCode_) return;

    QJsonObject obj;
    obj[JSON_ADDRESS] = webSocketServer_->getWebSocketServerNetworkAdress();
    obj[JSON_SECURITY_CODE] = securityCode_->getSecurityCode();

    QJsonDocument doc;
    doc.setObject(obj);

    QString qrCodeText = doc.toJson(QJsonDocument::Compact);

    ArkEchoQr* qrDialog = new ArkEchoQr(qrCodeText);
    qrDialog->show();
}

void ArkEchoPlayerModel::onTextMessageReceived(const WsStringData& message)
{
    if (!securityCode_) return;
    if (message = securityCode_->getSecurityCode())
    {
        // Blub
    }
}