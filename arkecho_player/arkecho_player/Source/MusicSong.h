#ifndef MUSICSONG_H
#define MUSICSONG_H

#include <QObject>
#include <QUrl>
#include <QMediaPlayer>

class MusicSong : public QObject
{
    Q_OBJECT

public:
    MusicSong(QUrl url, QObject* parent = 0);
    ~MusicSong();

    bool isLoaded();

    QUrl getUrl(); 

    QString getSongTitle();
    QString getSongInterpret();
    QString getSongAlbumTitle();
    QString getSongAlbumInterpret();

private slots:
    void onMediaStatusChanged(const QMediaPlayer::MediaStatus status);

private:
    bool loaded_;

    QUrl url_;
    QString songTitle_;
    QString songInterpret_;
    QString songAlbumTitle_;
    QString songAlbumInterpret_;

    QMediaPlayer* mp_;
};

#endif // MUSICSONG_H
