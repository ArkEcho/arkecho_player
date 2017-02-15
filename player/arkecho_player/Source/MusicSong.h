#ifndef MUSICSONG_H
#define MUSICSONG_H

#include <QObject>
#include <QUrl>
#include <QMediaPlayer>
#include <QImage>

class MusicSong : public QObject
{
    Q_OBJECT

    QUrl url_;
    QMediaPlayer::MediaStatus status_;
    QMediaPlayer* mp_;

    QString songTitle_;
    QString songInterpret_;
    qint64 songDuration_;

    int albumSongNumber_;
    int albumSongCount_;
    QString albumTitle_;
    QString albumInterpret_;
    QImage albumCoverArt_;

public:
    MusicSong(QUrl url, QObject* parent = 0);
    ~MusicSong();

    QUrl getUrl();
    QMediaPlayer::MediaStatus getStatus();

    QString getSongTitle();
    QString getSongInterpret();
    qint64 getSongDuration();
    QString getSongDurationAsMinuteSecond();

    int getAlbumSongNumber();
    int getAlbumSongCount();
    QString getAlbumTitle();
    QString getAlbumInterpret();
    QImage& getAlbumCoverArt();

    static QString convertMillisecondToMinuteSecond(qint64 millisecond);

private slots:
    void onMediaStatusChanged(const QMediaPlayer::MediaStatus status);
};

#endif // MUSICSONG_H
