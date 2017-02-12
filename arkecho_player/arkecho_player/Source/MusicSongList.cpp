#include "MusicSongList.h"

#include <QDirIterator>
#include <QMapIterator>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

const QString JSON_SONGLIST = "SongList";

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
        MusicSong* actualSong
            = songList_[actualKey];
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
    if (countNotLoaded == 0) return true;
    return false;
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
            int key = songList_.size() + 1; // Keys beginnen mit 1
            songList_.insert(songList_.size(),s);
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
        int newKey = newSongListSorted.size() + 1;
        newSongListSorted.insert(newKey, songList_[lowestAlbumSongKey]);
        songList_.remove(lowestAlbumSongKey);
    }
    songList_ = newSongListSorted;
}

void MusicSongList::toJSONString(QString& json)
{
    int listeSize = songList_.size();
    if (listeSize == 0) return;

    QJsonArray arr;
    QString string;
    QMapIterator<int, MusicSong*> it(songList_);
    while (it.hasNext())
    {
        MusicSong* s = it.next().value();
        if (!s)continue;
        s->toJSONString(string);
        arr.append(string);
    }
    if (arr.size() == 0) return;

    QJsonObject obj;
    obj[JSON_SONGLIST] = arr;

    QJsonDocument doc(obj);
    json = doc.toJson(QJsonDocument::Compact);
}

void MusicSongList::setFromJSONString(QString & json)
{
    QJsonDocument doc;
    doc = doc.fromJson(json.toUtf8());

    QJsonObject obj = doc.object();
    QJsonArray arr = obj[JSON_SONGLIST].toArray();
    int arraySize = arr.size();
    if (arraySize == 0) return;

    QString jsonSong;
    for (int i = 0; i < arraySize; ++i)
    {
        jsonSong = arr[i].toString();
        MusicSong *song = new MusicSong();
        song->setFromJSONString(jsonSong);
        songList_.insert(songList_.size(), song);
    }
}

QMap<int,MusicSong*> MusicSongList::getSongList()
{
    return songList_;
}
