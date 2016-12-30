#ifndef MUSICSONGLIST_H
#define MUSICSONGLIST_H

#include <QObject>
#include <QList>

class MusicSong;

class MusicSongList : public QObject
{
    Q_OBJECT

public:
    MusicSongList(QStringList directories, QObject *parent = 0);
    ~MusicSongList();

    bool allSongsLoaded();

private:
    QList<MusicSong*> songList_;
};

#endif // MUSICSONGLIST_H