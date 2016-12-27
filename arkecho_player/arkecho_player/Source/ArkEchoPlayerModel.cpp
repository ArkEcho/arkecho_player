#include "ArkEchoPlayerModel.h"
#include "ArkEchoQr.h"
#include "WebSocketServer.h"
#include "SecurityCode.h"
#include "MessageHandler.h"

#include <QJsonObject>
#include <QJsonDocument>
#include <QMessageBox>
#include <QWebSocket>

const QString SERVER_NAME = "ArkEcho Server";
const int SERVER_PORT = 1000;
const QString JSON_ADDRESS = "Address";
const QString JSON_SECURITY_CODE = "Security_Code";

ArkEchoPlayerModel::ArkEchoPlayerModel(QObject *parent)
    : QObject(parent)
{
    securityCode_ = new SecurityCode();

    webSocketServer_ = new WebSocketServer(SERVER_NAME);
    if (webSocketServer_->listen(QHostAddress::Any, SERVER_PORT)) // Port festlegen
    {
        //QString s = webSocketServer_->getWebSocketServerNetworkAdress();
        connect(webSocketServer_, SIGNAL(newTextMessageReceived(WsStringData)), this, SLOT(onTextMessageReceived(WsStringData)));
        connect(webSocketServer_, SIGNAL(wsConnected()), this, SLOT(onWSConnected()));
        connect(webSocketServer_, SIGNAL(wsDisconnected()), this, SLOT(onWSDisconnected()));
    }
}

ArkEchoPlayerModel::~ArkEchoPlayerModel()
{
    if(webSocketServer_) webSocketServer_->close();

    delete webSocketServer_;
    delete securityCode_;
}

void ArkEchoPlayerModel::showConnectQrDialog()
{
    if (!webSocketServer_ || !securityCode_) return;

    QString address = webSocketServer_->getWebSocketServerNetworkAdress() + ":" + QString::number(SERVER_PORT);
    QJsonObject obj;
    obj[JSON_ADDRESS] = address;
    obj[JSON_SECURITY_CODE] = securityCode_->getSecurityCode();

    QJsonDocument doc;
    doc.setObject(obj);

    QString qrCodeText = doc.toJson(QJsonDocument::Compact);

    ArkEchoQr* qrDialog = new ArkEchoQr(qrCodeText);
    qrDialog->show();
}

void ArkEchoPlayerModel::showConnectManualDialog()
{
    QString address = "Adresse:\t\t" + webSocketServer_->getWebSocketServerNetworkAdress() + ":" + QString::number(SERVER_PORT);
    QString secCode = "Sicherheitscode:\t" + QString::number(securityCode_->getSecurityCode());
    QMessageBox msgBox;
    msgBox.setWindowTitle("Verbindung Manuell herstellen");
    msgBox.setText(address + "\n" + secCode);
    msgBox.exec();
}

void ArkEchoPlayerModel::onWSConnected()
{
    emit updateView(UVE_WEBSOCKET_CONNECTED);
}

void ArkEchoPlayerModel::onWSDisconnected()
{
    emit updateView(UVE_WEBSOCKET_DISCONNECTED);
}

void ArkEchoPlayerModel::onTextMessageReceived(const WsStringData& data)
{
    QString message = data.message_;
    int messageType = MessageHandler::handleReceivedMessage(message);

    //if (wsSecCodeWasCorrect_)
    //{
    //    switch (messageType)
    //    {
    //        case MT_ECHO_TEST:
    //            webSocketServer_->getWebSocket()->sendTextMessage(MessageHandler::createMessage(MT_ECHO_TEST, message));
    //            break;
    //    }
    //}
    //else
    //{
    //    if (messageType == MT_HANDSHAKE_SEC_CODE && message.toInt() == securityCode_->getSecurityCode()) wsSecCodeWasCorrect_ = true;
    //    // TODO: Bei falschem Security Code Verbindung trennen
    //}
}