#ifndef MUSICSONGLIST_H
#define MUSICSONGLIST_H

#include "MusicSong.h"

#include <QObject>
#include <QMap>

class MusicSongList : public QObject
{
    Q_OBJECT

    QMap<int,MusicSong*> songList_;

public:
    MusicSongList(QObject *parent = 0);
    ~MusicSongList();

    bool allSongsLoaded();
    void loadSongs(QStringList& directories, QStringList& formats);

    // Sortiert in Reihenfolge AlbumTitel ABC -> AlbumNummer 123; vergibt die Keys neu
    void sortSongs();

    QMap<int,MusicSong*> getSongList();
};

#endif // MUSICSONGLIST_H