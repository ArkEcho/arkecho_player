#include "MediaFile.h"

MediaFile::MediaFile(QUrl url, QObject *parent)
    :url_(url)
    ,QObject(parent)
{
    mp_ = new QMediaPlayer();
    mp_->setMedia(url);

    // Ist die Datei geladen wird der Slot ausgelöst und die MetaDaten geladen
    connect(mp_, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)), this, SLOT(onMediaStatusChanged(QMediaPlayer::MediaStatus)));
}

MediaFile::~MediaFile()
{
}

QUrl MediaFile::getUrl()
{
    return url_;
}

QMediaPlayer::MediaStatus MediaFile::getStatus()
{
    return status_;
}

void MediaFile::onMediaStatusChanged(const QMediaPlayer::MediaStatus status)
{
    if (!mp_) return;
    status_ = status;
    if (status_ == QMediaPlayer::MediaStatus::LoadedMedia)// || status_ == QMediaPlayer::MediaStatus::BufferedMedia)
    {
        mediaLoaded();
    }
    mp_->deleteLater();
}

