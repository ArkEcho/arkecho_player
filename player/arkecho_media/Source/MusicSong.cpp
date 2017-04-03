#include "MusicSong.h"

#include <QMediaMetaData>
#include <QTime>

MusicSong::MusicSong(QUrl url, QObject* parent)
    :songDuration_(0)
    ,albumSongNumber_(0)
    ,albumSongCount_(0)
    ,MediaFile(url)
{
}

MusicSong::~MusicSong()
{
}

void MusicSong::mediaLoaded()
{
    songTitle_ = mp_->metaData(QMediaMetaData::Title).toString();
    songInterpret_ = mp_->metaData(QMediaMetaData::Author).toString();
    songDuration_ = mp_->metaData(QMediaMetaData::Duration).value<qint64>();

    albumSongNumber_ = mp_->metaData(QMediaMetaData::TrackNumber).toInt();
    albumSongCount_ = mp_->metaData(QMediaMetaData::TrackCount).toInt();
    albumTitle_ = mp_->metaData(QMediaMetaData::AlbumTitle).toString();
    albumInterpret_ = mp_->metaData(QMediaMetaData::AlbumArtist).toString();
    albumCoverArt_ = mp_->metaData(QMediaMetaData::ThumbnailImage).value<QImage>();
}

QString MusicSong::getSongTitle()
{
    return songTitle_;
}

QString MusicSong::getSongInterpret()
{
    return songInterpret_;
}

qint64 MusicSong::getSongDuration()
{
    return songDuration_;
}

QString MusicSong::getSongDurationAsMinuteSecond()
{
    return convertMillisecondToMinuteSecond(songDuration_);
}

int MusicSong::getAlbumSongNumber()
{
    return albumSongNumber_;
}

int MusicSong::getAlbumSongCount()
{
    return albumSongCount_;
}

QString MusicSong::getAlbumTitle()
{
    return albumTitle_;
}

QString MusicSong::getAlbumInterpret()
{
    return albumInterpret_;
}

QImage & MusicSong::getAlbumCoverArt()
{
    return albumCoverArt_;
}

QString MusicSong::convertMillisecondToMinuteSecond(qint64 millisecond)
{
    int secondsTotal = millisecond / 1000;
    int minutes = secondsTotal / 60;
    int seconds = secondsTotal % 60;
    QString secondsString = "";
    if (seconds < 10) secondsString = "0" + QString::number(seconds);
    else secondsString = QString::number(seconds);
    QString duration = QString::number(minutes) + ":" + secondsString;
    return duration;
}