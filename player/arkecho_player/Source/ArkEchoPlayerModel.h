#ifndef ARKECHOPLAYERMODEL_H
#define ARKECHOPLAYERMODEL_H

#include <QObject>
#include <QImage>
#include <QMap>

class WebSocketServer;
class MusicSongList;
class QMediaPlaylist;

enum UpdateViewEnum
{
    TEST = 0,
    WEBSOCKET_CONNECTED,
    WEBSOCKET_DISCONNECTED,
    REMOTE_BUTTON_BACKWARD,
    REMOTE_BUTTON_FORWARD,
    REMOTE_BUTTON_PLAY_PAUSE
};

struct SongInfoStruct
{
    QString songTitle_;
    QString songInterpret_;
    QString albumTitle_;
    QString albumInterpret_;
    QImage coverArt_;
};

class ArkEchoPlayerModel : public QObject
{
    Q_OBJECT

    QMap<int, int> playlistIndexSongListKeyMap;
    WebSocketServer* webSocketServer_;
    MusicSongList* musicSongList_;
    QMediaPlaylist* playlist_;

    // Anhand einer Playlist Position die SongInfo im View aktualisieren und Senden
    void setActualSongInfoAndSend(int playlistPosition);

public:
    ArkEchoPlayerModel(QObject *parent = 0);
    ~ArkEchoPlayerModel();

    // Löschen und Neu Laden der SongList anhand der Dir und Formate
    void loadMusicSongList();

    void showConnectQrDialog();
    void showConnectManualDialog();
    void showSettingsDialog();

    void setMediaPlaylist(QList<int>& keys, int selectedKey);
    void backwardPlaylist();
    void forwardPlaylist();
    void shufflePlaylist();

    QMediaPlaylist* getMediaPlaylist();
    MusicSongList* getMusicSongList();
    QString getWebServerAddress();

signals:
    void updateView(int);
    void actualSongInfoChanged(SongInfoStruct);

private slots:
    void onTextMessageReceived(const QString& message);
    void onWSConnected();
    void onWSDisconnected();
    void onPlaylistCurrentIndexChanged(const int& position);
};

#endif // ARKECHOPLAYERMODEL_H
