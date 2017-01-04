#include "ArkEchoPlayerModel.h"
#include "ArkEchoQr.h"
#include "WebSocketServer.h"
#include "MessageHandler.h"
#include "MusicSongList.h"

#include <QMessageBox>
#include <QWebSocket>
#include <QDir>
#include <QMediaPlaylist>
#include <QJsonObject>
#include <QJsonDocument>
#include <QBuffer>

const QString SERVER_NAME = "ArkEcho Server";
const int SERVER_PORT = 1000;
const QString JSON_COVER_ART = "CoverArt";
const QString JSON_SONG_TITLE = "SongTitle";
const QString JSON_SONG_INTERPRET = "SongInterpret";
const QString JSON_ALBUM_TITLE = "AlbumTitle";
const QString JSON_ALBUM_INTERPRET = "AlbumInterpret";

ArkEchoPlayerModel::ArkEchoPlayerModel(QObject *parent)
    :QObject(parent)
    ,musicSongList_(0)
    ,webSocketServer_(0)
    ,playlist_(0)
{
    musicSongList_ = new MusicSongList();
    musicSongList_->loadSongs(getMusicDirectoryList());
    while (!musicSongList_->allSongsLoaded())
    {
        qApp->processEvents();
    }

    webSocketServer_ = new WebSocketServer(SERVER_NAME);
    if (webSocketServer_->listen(QHostAddress::Any, SERVER_PORT)) // Port festlegen
    {
        //QString s = webSocketServer_->getWebSocketServerNetworkAdress();
        connect(webSocketServer_, SIGNAL(newTextMessageReceived(QString)), this, SLOT(onTextMessageReceived(QString)));
        connect(webSocketServer_, SIGNAL(wsConnected()), this, SLOT(onWSConnected()));
        connect(webSocketServer_, SIGNAL(wsDisconnected()), this, SLOT(onWSDisconnected()));
    }

    playlist_ = new QMediaPlaylist();
}

ArkEchoPlayerModel::~ArkEchoPlayerModel()
{
    if(webSocketServer_) webSocketServer_->close();

    delete webSocketServer_;
    delete musicSongList_;
    delete playlist_;
}

void ArkEchoPlayerModel::showConnectQrDialog()
{
    if (!webSocketServer_) return;

    ArkEchoQr* qrDialog = new ArkEchoQr(getWebServerAddress());
    qrDialog->exec();
    qrDialog->deleteLater();
}

void ArkEchoPlayerModel::showConnectManualDialog()
{
    QString address = "Adresse:\t" + getWebServerAddress() +"\t";
    QMessageBox msgBox;
    msgBox.setWindowTitle("Verbindung Manuell herstellen");
    msgBox.setText("\n" + address + "\n");
    msgBox.exec();
}

void ArkEchoPlayerModel::setMediaPlaylist(QList<int> keys, int selectedKey)
{
    if (!musicSongList_ || !playlist_) return;
    playlist_->clear();
    if (keys.size() == 0) return;

    int index = 0, startIndex = 0;
    QListIterator<int> itKeys(keys);
    while (itKeys.hasNext())
    {
        int keyList = itKeys.next();
        QMapIterator<int, MusicSong*> itSongs(musicSongList_->getSongList());
        while (itSongs.hasNext())
        {
            int keyMap = itSongs.next().key();
            if (keyList == keyMap)
            {
                playlist_->insertMedia(index, musicSongList_->getSongList().value(keyList)->getMediaContent());
                if (keyList == selectedKey) startIndex = index;
                ++index;
                break;
            }
        }
    }
    playlist_->setCurrentIndex(startIndex);
}

void ArkEchoPlayerModel::backwardPlaylist()
{
    if (!playlist_) return;
    playlist_->previous();
}

void ArkEchoPlayerModel::forwardPlaylist()
{
    if (!playlist_) return;
    playlist_->next();
}

void ArkEchoPlayerModel::shufflePlaylist()
{
    if (!playlist_) return;
    playlist_->shuffle();
}

void ArkEchoPlayerModel::sendActualSongInfo(QImage image, QString songTitle, QString songInterpret, QString albumTitle, QString albumInterpret)
{
    QJsonObject obj;

    QByteArray ba;
    QBuffer bu(&ba);

    image.save(&bu, "PNG");

    QString imgBase64 = ba.toBase64();
    obj[JSON_COVER_ART] = imgBase64;
    obj[JSON_SONG_TITLE] = songTitle;
    obj[JSON_SONG_INTERPRET] = songInterpret;
    obj[JSON_ALBUM_TITLE] = albumTitle;
    obj[JSON_ALBUM_INTERPRET] = albumInterpret;

    QJsonDocument doc;
    doc.setObject(obj);

    QString message = doc.toJson(QJsonDocument::Compact);
    webSocketServer_->sendMessage(MT_SONG_ACTUAL, message);
}

QMediaPlaylist * ArkEchoPlayerModel::getMediaPlaylist()
{
    return playlist_;
}

MusicSongList * ArkEchoPlayerModel::getMusicSongList()
{
    return musicSongList_;
}

QString ArkEchoPlayerModel::getWebServerAddress()
{
    return webSocketServer_->getWebSocketServerNetworkAdress() + ":" + QString::number(SERVER_PORT);
}

QStringList ArkEchoPlayerModel::getMusicDirectoryList()
{
    QStringList list = QStringList() << QDir::homePath() + "/Music/";
    return list;
}

void ArkEchoPlayerModel::onWSConnected()
{
    emit updateView(UVE_WEBSOCKET_CONNECTED);
}

void ArkEchoPlayerModel::onWSDisconnected()
{
    emit updateView(UVE_WEBSOCKET_DISCONNECTED);
}

void ArkEchoPlayerModel::onTextMessageReceived(const QString& message)
{
    QString msg = message;
    int messageType = MessageHandler::handleReceivedMessage(msg);

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