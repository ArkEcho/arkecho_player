#ifndef MUSICSONG_H
#define MUSICSONG_H

#include <QObject>
#include <QUrl>
#include <QMediaPlayer>
#include <QImage>

class QMediaPlayer;

class MusicSong : public QObject
{
    Q_OBJECT

public:
    MusicSong(QUrl url, QObject* parent = 0);
    ~MusicSong();

    QUrl getUrl();

private slots:
    void onMediaStatusChanged(const QMediaPlayer::MediaStatus status);

private:
    QUrl url_;

    QString songTitle_;
    QString songInterpret_;
    QString songAlbumTitle_;
    QString songAlbumInterpret_;

    QImage songCoverArt_;

    QMediaPlayer* mp_;
};

#endif // MUSICSONG_H
