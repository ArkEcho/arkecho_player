#ifndef MEDIAFILE_H
#define MEDIAFILE_H

#include "arkecho_media_global.h"
#include <QObject>
#include <QUrl>
#include <QMediaPlayer>

class ARKECHO_MEDIA_EXPORT MediaFile : public QObject
{
    Q_OBJECT

protected:
    QUrl url_;
    QMediaPlayer::MediaStatus status_;
    QMediaPlayer* mp_;

    virtual void mediaLoaded() = 0;

public:
    MediaFile(QUrl url, QObject *parent = 0);
    ~MediaFile();

    QUrl getUrl();
    QMediaPlayer::MediaStatus getMediaStatus();

private slots:
    void onMediaStatusChanged(const QMediaPlayer::MediaStatus status);
};

#endif // MEDIAFILE_H
