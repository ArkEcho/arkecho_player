#include "ArkEchoPlayerModel.h"
#include "ArkEchoQr.h"
#include "WebSocketServer.h"
#include "MessageHandler.h"
//#include "MusicSong.h"

#include <QMessageBox>
#include <QWebSocket>

const QString SERVER_NAME = "ArkEcho Server";
const int SERVER_PORT = 1000;

ArkEchoPlayerModel::ArkEchoPlayerModel(QObject *parent)
    : QObject(parent)
{
    /*QString path = "C:/Users/steph/Music/Breaking Bad Soundtrack/TV On The Radio - DLZ.mp3";
    MusicSong* s = new MusicSong(QUrl::fromLocalFile(path));*/
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
}

void ArkEchoPlayerModel::showConnectQrDialog()
{
    if (!webSocketServer_) return;

    ArkEchoQr* qrDialog = new ArkEchoQr(getWebServerAddress());
    qrDialog->show();
}

void ArkEchoPlayerModel::showConnectManualDialog()
{
    QString address = "Adresse:\t" + getWebServerAddress() +"\t";
    QMessageBox msgBox;
    msgBox.setWindowTitle("Verbindung Manuell herstellen");
    msgBox.setText("\n" + address + "\n");
    msgBox.exec();
}

QString ArkEchoPlayerModel::getWebServerAddress()
{
    return webSocketServer_->getWebSocketServerNetworkAdress() + ":" + QString::number(SERVER_PORT);
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
}