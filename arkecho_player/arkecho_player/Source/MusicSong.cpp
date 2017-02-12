#include "MusicSong.h"

#include <QMediaMetaData>
#include <QTime>
#include <QImage>
#include <QJsonObject>
#include <QJsonDocument>

MusicSong::MusicSong(QObject * parent)
{
    status_ = QMediaPlayer::MediaStatus::LoadedMedia;
}

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

QString MusicSong::getSongTitle(QMediaPlayer * mp)
{
    if (!mp) return "";
    return getMetaDataSongTitle(mp);
}

QString MusicSong::getSongInterpret(QMediaPlayer * mp)
{
    if (!mp) return "";
    return getMetaDataSongInterpret(mp);
}

qint64 MusicSong::getSongDuration(QMediaPlayer * mp)
{
    if (!mp) return 0;
    return getMetaDataSongDuration(mp);
}

QString MusicSong::getSongDurationAsMinuteSecond(QMediaPlayer * mp)
{
    if (!mp) return "";
    return convertMillisecondToMinuteSecond(getMetaDataSongDuration(mp));
}

int MusicSong::getAlbumSongNumber(QMediaPlayer * mp)
{
    if (!mp) return 0;
    return getMetaDataAlbumSongNumber(mp);
}

int MusicSong::getAlbumSongCount(QMediaPlayer * mp)
{
    if (!mp) return 0;
    return getMetaDataAlbumSongCount(mp);
}

QString MusicSong::getAlbumTitle(QMediaPlayer * mp)
{
    if (!mp) return "";
    return getMetaDataAlbumTitle(mp);
}

QString MusicSong::getAlbumInterpret(QMediaPlayer * mp)
{
    if (!mp) return "";
    return getMetaDataAlbumInterpret(mp);
}

QImage MusicSong::getAlbumCoverArt(QMediaPlayer * mp)
{
    if (!mp) QImage();
    return getMetaDataAlbumCoverArt(mp);
}

void MusicSong::onMediaStatusChanged(const QMediaPlayer::MediaStatus status)
{
    if (!mp_) return;
    status_ = status;
    if (status_ == QMediaPlayer::MediaStatus::LoadedMedia || status_ == QMediaPlayer::MediaStatus::BufferedMedia)
    {
        songTitle_ = getMetaDataSongTitle(mp_);
        songInterpret_ = getMetaDataSongInterpret(mp_);
        songDuration_ = getMetaDataSongDuration(mp_);

        albumSongNumber_ = getMetaDataAlbumSongNumber(mp_);
        albumSongCount_ = getMetaDataAlbumSongCount(mp_);
        albumTitle_ = getMetaDataAlbumTitle(mp_);
        albumInterpret_ = getMetaDataAlbumInterpret(mp_);
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

QString MusicSong::getMetaDataSongTitle(QMediaPlayer * mp)
{
    return mp->metaData(QMediaMetaData::Title).toString();
}

QString MusicSong::getMetaDataSongInterpret(QMediaPlayer * mp)
{
    return mp->metaData(QMediaMetaData::Author).toString();
}

qint64 MusicSong::getMetaDataSongDuration(QMediaPlayer * mp)
{
    return mp->metaData(QMediaMetaData::Duration).value<qint64>();
}

int MusicSong::getMetaDataAlbumSongNumber(QMediaPlayer * mp)
{
    return mp->metaData(QMediaMetaData::TrackNumber).toInt();
}

int MusicSong::getMetaDataAlbumSongCount(QMediaPlayer * mp)
{
    return mp->metaData(QMediaMetaData::TrackCount).toInt();
}

QString MusicSong::getMetaDataAlbumTitle(QMediaPlayer * mp)
{
    return mp->metaData(QMediaMetaData::AlbumTitle).toString();
}

QString MusicSong::getMetaDataAlbumInterpret(QMediaPlayer * mp)
{
    return mp->metaData(QMediaMetaData::AlbumArtist).toString();
}

QImage MusicSong::getMetaDataAlbumCoverArt(QMediaPlayer * mp)
{
    return mp->metaData(QMediaMetaData::ThumbnailImage).value<QImage>();
}
