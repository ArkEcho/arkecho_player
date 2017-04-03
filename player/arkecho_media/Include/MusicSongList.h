#ifndef MUSICSONGLIST_H
#define MUSICSONGLIST_H

#include "arkecho_media_global.h"
#include "MusicSong.h"
#include <QObject>
#include <QMap>
#include <QJsonObject>

class ARKECHO_MEDIA_EXPORT MusicSongList : public QObject
{
    Q_OBJECT

    QMap<int,MusicSong*> songList_;
    void songToJSONObject(MusicSong* song,QJsonObject& obj, bool coverArt = false);

public:
    MusicSongList(QObject *parent = 0);
    ~MusicSongList();

    // Sortiert in Reihenfolge AlbumTitel ABC -> AlbumNummer 123; vergibt die Keys neu
    void sortSongs();
    bool allSongsLoaded();
    void loadSongs(QStringList& directories, QStringList& formats);

    // List to JSON with Keys, no CoverArt
    void toJSONString(QString& json);

    // Song to JSON by Map Key, coverArt with bool
    void songToJSONString(int key, QString& json, bool coverArt);
    
    QMap<int,MusicSong*> getSongList();
};

#endif // MUSICSONGLIST_H