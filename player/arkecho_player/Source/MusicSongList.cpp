#include "MusicSongList.h"

#include <QDirIterator>
#include <QMapIterator>
#include <QBuffer>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

const QString JSON_SONGLIST = "SongList";
const QString JSON_SONG = "Song";
const QString JSON_KEY = "Key";
const QString JSON_SONGTITLE = "SongTitle";
const QString JSON_SONGINTERPRET = "SongInterpret";
const QString JSON_SONGDURATION = "SongDuration";
const QString JSON_ALBUMSONGNUMBER = "AlbumSongNumber";
const QString JSON_ALBUMSONGCOUNT = "AlbumSongCount";
const QString JSON_ALBUMTITLE = "AlbumTitle";
const QString JSON_ALBUMINTERPRET = "AlbumInterpret";
const QString JSON_ALBUMCOVERART = "CoverArt";

void MusicSongList::songToJSONObject(MusicSong * song, QJsonObject& obj, bool coverArt)
{
    obj[JSON_SONGTITLE] = song->getSongTitle();
    obj[JSON_SONGINTERPRET] = song->getSongInterpret();
    obj[JSON_SONGDURATION] = song->getSongDuration();
    obj[JSON_ALBUMSONGNUMBER] = song->getAlbumSongNumber();
    obj[JSON_ALBUMSONGCOUNT] = song->getAlbumSongCount();
    obj[JSON_ALBUMTITLE] = song->getAlbumTitle();
    obj[JSON_ALBUMINTERPRET] = song->getAlbumInterpret();
    if (coverArt)
    {
        QByteArray ba;
        QBuffer bu(&ba);
        song->getAlbumCoverArt().save(&bu, "PNG");
        obj[JSON_ALBUMCOVERART] = (QString)ba.toBase64();
    }
}

MusicSongList::MusicSongList(QObject *parent)
    : QObject(parent)
{
}

MusicSongList::~MusicSongList()
{
    qDeleteAll(songList_);
}

bool MusicSongList::allSongsLoaded()
{
    if (songList_.size() == 0) return true;
    int countNotLoaded = 0;
    QMapIterator<int,MusicSong*> it(songList_);
    while (it.hasNext())
    {
        int actualKey = it.next().key();
        MusicSong* actualSong = songList_[actualKey];
        if (!actualSong) continue;
        if (actualSong->getStatus() == QMediaPlayer::MediaStatus::InvalidMedia)
        {
            delete actualSong;
            songList_.remove(actualKey);
        }
        else if (actualSong->getStatus() != QMediaPlayer::MediaStatus::LoadedMedia)
        {
            ++countNotLoaded;
        }
    }
    if (countNotLoaded > 0) return false;
    else return true;
}

void MusicSongList::loadSongs(QStringList& directories, QStringList& formats)
{
    qDeleteAll(songList_);
    QDirIterator* it = 0;
    for (int i = 0; i < directories.size(); ++i)
    {
        it = new QDirIterator(directories.at(i), formats, QDir::Files, QDirIterator::Subdirectories);
        while (it->hasNext())
        {
            MusicSong* s = new MusicSong(QUrl::fromLocalFile(it->next()));
            int key = songList_.size();
            songList_.insert(key,s);
        }
    }
    delete it;
}

void MusicSongList::sortSongs()
{
    QMap<int, MusicSong*> newSongListSorted;
    int songListSize = songList_.size();
    while (newSongListSorted.size() < songListSize)
    {
        QString lowestAlbumTitle = "ZZZZZZZZZZ";
        int lowestAlbumSongNumber = 9999999;
        int lowestAlbumSongKey = 0;
        QMapIterator<int, MusicSong*> iteratorOldList(songList_);
        while (iteratorOldList.hasNext())
        {
            int actualKey = iteratorOldList.next().key();
            MusicSong* actualSong = songList_[actualKey];
            if (!actualSong) continue;
            QString actualAlbumTitle = actualSong->getAlbumTitle();
            int actualAlbumSongNumber = actualSong->getAlbumSongNumber();

            if (actualAlbumTitle < lowestAlbumTitle || (actualAlbumTitle == lowestAlbumTitle && actualAlbumSongNumber < lowestAlbumSongNumber))
            {
                lowestAlbumTitle = actualAlbumTitle;
                lowestAlbumSongNumber = actualAlbumSongNumber;
                lowestAlbumSongKey = actualKey;
            }
        }
        int newKey = newSongListSorted.size();
        newSongListSorted.insert(newKey, songList_[lowestAlbumSongKey]);
        songList_.remove(lowestAlbumSongKey);
    }
    // Überschreiben der alten Liste mit der sortierten
    songList_ = newSongListSorted;
}

void MusicSongList::toJSONString(QString& json)
{
    int listeSize = songList_.size();
    if (listeSize == 0) return;

    QJsonArray arr;
    QString songJSON;
    QMapIterator<int, MusicSong*> it(songList_);
    while (it.hasNext())
    {
        int key = it.next().key();
        MusicSong* song = songList_.value(key);
        if (!song)continue;
        QJsonObject obj;
        songToJSONObject(song, obj);

        QJsonObject objSong;
        objSong[JSON_KEY] = key;
        objSong[JSON_SONG] = obj;
        arr.append(objSong);
    }
    if (arr.size() == 0) return;

    QJsonObject obj;
    obj[JSON_SONGLIST] = arr;

    QJsonDocument doc(obj);
    json = doc.toJson(QJsonDocument::Compact);
}

void MusicSongList::songToJSONString(int key, QString & json, bool coverArt)
{
    QJsonObject obj;
    songToJSONObject(songList_.value(key), obj, coverArt);

    QJsonDocument doc(obj);
    json = doc.toJson(QJsonDocument::Compact);
}

QMap<int,MusicSong*> MusicSongList::getSongList()
{
    return songList_;
}
