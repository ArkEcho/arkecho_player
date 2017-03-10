#ifndef MUSICSONG_H
#define MUSICSONG_H

#include "MediaFile.h"
#include <QImage>

class MusicSong : public MediaFile
{
    Q_OBJECT

    QString songTitle_;
    QString songInterpret_;
    qint64 songDuration_;

    int albumSongNumber_;
    int albumSongCount_;
    QString albumTitle_;
    QString albumInterpret_;
    QImage albumCoverArt_;

    void mediaLoaded();

public:
    MusicSong(QUrl url, QObject* parent = 0);
    ~MusicSong();

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
};

#endif // MUSICSONG_H
