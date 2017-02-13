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
    musicSongList_->loadSongs(getMusicDirectoryList(), getMusicFormatList());
    while (!musicSongList_->allSongsLoaded())
    {
        qApp->processEvents();
    }
    musicSongList_->sortSongs();
    //QString json;
    //musicSongList_->toJSONString(json);
    //musicSongList_->songToJSONString(2, json, true);

    webSocketServer_ = new WebSocketServer(SERVER_NAME);
    if (webSocketServer_->listen(QHostAddress::Any, SERVER_PORT)) // Port festlegen
    {
        //QString s = webSocketServer_->getWebSocketServerNetworkAdress();
        connect(webSocketServer_, SIGNAL(newTextMessageReceived(QString)), this, SLOT(onTextMessageReceived(QString)));
        connect(webSocketServer_, SIGNAL(wsConnected()), this, SLOT(onWSConnected()));
        connect(webSocketServer_, SIGNAL(wsDisconnected()), this, SLOT(onWSDisconnected()));
    }

    playlist_ = new QMediaPlaylist();
    connect(playlist_, SIGNAL(currentIndexChanged(int)), this, SLOT(onPlaylistCurrentIndexChanged(int)));
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

void ArkEchoPlayerModel::setMediaPlaylist(QList<int>& keys, int selectedKey)
{
    if (!musicSongList_ || !playlist_) return;
    playlist_->clear();
    if (keys.size() == 0) return;

    QMap<int, MusicSong*> songList = musicSongList_->getSongList();
    int index = 0, startIndex = 0;
    QListIterator<int> itKeys(keys);
    while (itKeys.hasNext())
    {
        int keyList = itKeys.next();
        QMapIterator<int, MusicSong*> itSongs(songList);
        while (itSongs.hasNext())
        {
            int keyMap = itSongs.next().key();
            if (keyList == keyMap)
            {
                playlist_->insertMedia(index, songList.value(keyList)->getUrl());
                playlistIndexSongListKeyMap.insert(index, keyMap);
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
    if (!webSocketServer_) return "";
    return webSocketServer_->getWebSocketServerNetworkAdress() + ":" + QString::number(SERVER_PORT);
}

QStringList ArkEchoPlayerModel::getMusicDirectoryList()
{
    QStringList list = QStringList() << QDir::homePath() + "/Music/";
    return list;
}

QStringList ArkEchoPlayerModel::getMusicFormatList()
{
    QStringList list = QStringList() << "*.mp3" << "*.m4a";
    return list;
}

void ArkEchoPlayerModel::onWSConnected()
{
    emit updateView(WEBSOCKET_CONNECTED);
}

void ArkEchoPlayerModel::onWSDisconnected()
{
    emit updateView(WEBSOCKET_DISCONNECTED);
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
    case MT_REQUEST_SONG_ACTUAL:
        emit updateView(REQUEST_SONG_ACTUAL_BY_SOCKET);
        break;
    }
}

void ArkEchoPlayerModel::onPlaylistCurrentIndexChanged(const int & position)
{
    if (!musicSongList_) return;
    // Zur�ck mapping der Playlist Position auf den Key der Songlist
    int songListKey = playlistIndexSongListKeyMap.value(position);
    MusicSong* song = musicSongList_->getSongList().value(songListKey);
    if (!song) return;
    SongInfoStruct sis;
    sis.songTitle_ = song->getSongTitle();
    sis.songInterpret_ = song->getSongInterpret();
    sis.albumTitle_ = song->getAlbumTitle();
    sis.albumInterpret_ = song->getAlbumInterpret();
    sis.coverArt_ = song->getAlbumCoverArt();
    emit actualSongInfoChanged(sis);

    // Sending of actual Song Info
    if (!webSocketServer_) return;
    if (!webSocketServer_->checkIfConnectionIsOpen()) return;
    QString songAsJSON;
    musicSongList_->songToJSONString(songListKey, songAsJSON, true);
    webSocketServer_->sendMessage(MT_SEND_SONG_ACTUAL, songAsJSON);
}