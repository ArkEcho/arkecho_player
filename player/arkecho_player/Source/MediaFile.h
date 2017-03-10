#ifndef MEDIAFILE_H
#define MEDIAFILE_H

#include <QObject>
#include <QUrl>
#include <QMediaPlayer>

class MediaFile : public QObject
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
    QMediaPlayer::MediaStatus getStatus();

private slots:
    void onMediaStatusChanged(const QMediaPlayer::MediaStatus status);
};

#endif // MEDIAFILE_H
