#ifndef MUSICSONGLIST_H
#define MUSICSONGLIST_H

#include "MusicSong.h"

#include <QObject>
#include <QMap>

class MusicSongList : public QObject
{
    Q_OBJECT

public:
    MusicSongList(QObject *parent = 0);
    ~MusicSongList();

    bool allSongsLoaded();
    void loadSongs(QStringList directories);
    QMap<int,MusicSong*> getSongList();

private:
    QMap<int,MusicSong*> songList_;
};

#endif // MUSICSONGLIST_H