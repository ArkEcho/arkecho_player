#include "ArkEchoPlayerModel.h"
#include "ArkEchoQr.h"
#include "WebSocketServer.h"
#include "MessageHandler.h"

#include <QMessageBox>
#include <QWebSocket>

const QString SERVER_NAME = "ArkEcho Server";
const int SERVER_PORT = 1000;
const QStringList DIRECTORY_LIST = QStringList() << "C:/Users/steph/Music/";

ArkEchoPlayerModel::ArkEchoPlayerModel(QObject *parent)
    :QObject(parent)
    ,musicSongList_(0)
    ,webSocketServer_(0)
{
    musicSongList_ = new MusicSongList();
    musicSongList_->loadSongs(DIRECTORY_LIST);
    while (!musicSongList_->allSongsLoaded())
    {
        qApp->processEvents();
    }

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
    delete musicSongList_;
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

MusicSongList * ArkEchoPlayerModel::getMusicSongList()
{
    return musicSongList_;
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

    switch (messageType)
    {
    case MT_BACKWARD:
        emit updateView(REMOTE_BUTTON_BACKWARD);
        break;
    case MT_FORWARD:
        emit updateView(REMOTE_BUTTON_FORWARD);
        break;
    case MT_PLAY_PAUSE:
        emit updateView(REMOTE_BUTTON_PLAY_PAUSE);
        break;
    }
}