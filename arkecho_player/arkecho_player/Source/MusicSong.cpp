#include "MusicSong.h"

#include <QMediaMetaData>
#include <QTime>

MusicSong::MusicSong(QUrl url, QObject* parent)
    :url_(url)
    ,loaded_(false)
{
    mp_ = new QMediaPlayer();
    mp_->setMedia(url_);

    // Ist die Datei geladen wird der Slot ausgelöst und die MetaDaten geladen
    connect(mp_, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)), this, SLOT(onMediaStatusChanged(QMediaPlayer::MediaStatus)));
}

MusicSong::~MusicSong()
{
    delete mp_;
}

bool MusicSong::isLoaded()
{
    return loaded_;
}

QUrl MusicSong::getUrl()
{
    return url_;
}

QMediaContent MusicSong::getMediaContent()
{
    return mediaContent_;
}

QString MusicSong::getSongTitle()
{
    if (!mp_) return "";
    return getMetaDataSongTitle(mp_);
}

QString MusicSong::getSongInterpret()
{
    if (!mp_) return "";
    return getMetaDataSongInterpret(mp_);
}

qint64 MusicSong::getSongDuration()
{
    if (!mp_) return 0 ;
    return getMetaDataSongDuration(mp_);
}

QString MusicSong::getSongDurationAsMinuteSecond()
{
    if (!mp_) return "";
    return convertMillisecondToMinuteSecond(getMetaDataSongDuration(mp_));
}

int MusicSong::getAlbumSongNumber()
{
    if (!mp_) return 0;
    return getMetaDataAlbumSongNumber(mp_);
}

int MusicSong::getAlbumSongCount()
{
    if (!mp_) return 0;
    return getMetaDataAlbumSongCount(mp_);
}

QString MusicSong::getAlbumTitle()
{
    if (!mp_) return "";
    return getMetaDataAlbumTitle(mp_);
}

QString MusicSong::getAlbumInterpret()
{
    if (!mp_) return "";
    return getMetaDataAlbumInterpret(mp_);
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

void MusicSong::onMediaStatusChanged(const QMediaPlayer::MediaStatus status)
{
    if (status == QMediaPlayer::MediaStatus::LoadedMedia)
    {
        mediaContent_ = mp_->media();
        loaded_ = true;
        //songCoverArt_ = new QImage(mp_->metaData(QMediaMetaData::CoverArtImage).value<QImage>());
    }
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
