#include "MusicSong.h"

#include <QMediaMetaData>
#include <QTime>
#include <QJsonObject>
#include <QJsonDocument>

MusicSong::MusicSong(QUrl url, QObject* parent)
    :url_(url)
    ,songDuration_(0)
    ,albumSongNumber_(0)
    ,albumSongCount_(0)
{
    mp_ = new QMediaPlayer();
    mp_->setMedia(url);

    // Ist die Datei geladen wird der Slot ausgelöst und die MetaDaten geladen
    connect(mp_, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)), this, SLOT(onMediaStatusChanged(QMediaPlayer::MediaStatus)));
}

MusicSong::~MusicSong()
{
    //delete mp_;
}

QMediaPlayer::MediaStatus MusicSong::getStatus()
{
    return status_;
}

QUrl MusicSong::getUrl()
{
    return url_;
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

void MusicSong::onMediaStatusChanged(const QMediaPlayer::MediaStatus status)
{
    if (!mp_) return;
    status_ = status;
    if (status_ == QMediaPlayer::MediaStatus::LoadedMedia || status_ == QMediaPlayer::MediaStatus::BufferedMedia)
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
    mp_->deleteLater();
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