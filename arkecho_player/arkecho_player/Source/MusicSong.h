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
    QMediaContent getMediaContent();

    QString getSongTitle();
    QString getSongInterpret();
    qint64 getSongDuration();
    QString getSongDurationAsMinuteSecond();

    int getAlbumSongNumber();
    int getAlbumSongCount();
    QString getAlbumTitle();
    QString getAlbumInterpret();

    static QString getSongTitle(QMediaPlayer* mp);
    static QString getSongInterpret(QMediaPlayer* mp);
    static qint64 getSongDuration(QMediaPlayer* mp);
    static QString getSongDurationAsMinuteSecond(QMediaPlayer* mp);

    static int getAlbumSongNumber(QMediaPlayer* mp);
    static int getAlbumSongCount(QMediaPlayer* mp);
    static QString getAlbumTitle(QMediaPlayer* mp);
    static QString getAlbumInterpret(QMediaPlayer* mp);

    static QString convertMillisecondToMinuteSecond(qint64 millisecond);
private slots:
    void onMediaStatusChanged(const QMediaPlayer::MediaStatus status);

private:
    QUrl url_;

    bool loaded_;
    QMediaPlayer* mp_;
    QMediaContent mediaContent_;


    static QString getMetaDataSongTitle(QMediaPlayer* mp);
    static QString getMetaDataSongInterpret(QMediaPlayer* mp);
    static qint64 getMetaDataSongDuration(QMediaPlayer* mp);

    static int getMetaDataAlbumSongNumber(QMediaPlayer* mp);
    static int getMetaDataAlbumSongCount(QMediaPlayer* mp);
    static QString getMetaDataAlbumTitle(QMediaPlayer* mp);
    static QString getMetaDataAlbumInterpret(QMediaPlayer* mp);
};

#endif // MUSICSONG_H
