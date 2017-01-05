#ifndef ARKECHOPLAYERMODEL_H
#define ARKECHOPLAYERMODEL_H

#include <QObject>
#include <QImage>

class WebSocketServer;
class MusicSongList;
class QMediaPlaylist;

enum UpdateViewEnum
{
    TEST = 0,
    UVE_WEBSOCKET_CONNECTED,
    UVE_WEBSOCKET_DISCONNECTED,
    REMOTE_BUTTON_BACKWARD,
    REMOTE_BUTTON_FORWARD,
    REMOTE_BUTTON_PLAY_PAUSE,
    REQUEST_SONG_ACTUAL
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

    WebSocketServer* webSocketServer_;
    MusicSongList* musicSongList_;
    QMediaPlaylist* playlist_;

    QStringList getMusicDirectoryList();

public:
    ArkEchoPlayerModel(QObject *parent = 0);
    ~ArkEchoPlayerModel();

    void showConnectQrDialog();
    void showConnectManualDialog();

    void setMediaPlaylist(QList<int>& keys, int selectedKey);
    void backwardPlaylist();
    void forwardPlaylist();
    void shufflePlaylist();
    void sendActualSongInfo(SongInfoStruct& siStruct);

    QMediaPlaylist* getMediaPlaylist();
    MusicSongList* getMusicSongList();
    QString getWebServerAddress();

signals:
    void updateView(int);

private slots:
    void onTextMessageReceived(const QString& message);
    void onWSConnected();
    void onWSDisconnected();
};

#endif // ARKECHOPLAYERMODEL_H
