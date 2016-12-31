#include "MusicSongList.h"

#include <QDirIterator>
#include <QMapIterator>

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
    QMapIterator<int,MusicSong*> it(songList_);
    while (it.hasNext())
    {
        MusicSong* s = it.next().value();
        if (!s) continue;
        if (!s->isLoaded()) return false;
    }
    return true;
}

void MusicSongList::loadSongs(QStringList directories)
{
    qDeleteAll(songList_);
    QDirIterator* it = 0;
    for (int i = 0; i < directories.size(); ++i)
    {
        QDirIterator* it = new QDirIterator(directories.at(i), QStringList() << "*.mp3", QDir::Files, QDirIterator::Subdirectories);
        while (it->hasNext())
        {
            MusicSong* s = new MusicSong(QUrl::fromLocalFile(it->next()));
            songList_.insert(songList_.size(),s);
        }
    }
    delete it;
}

QMap<int,MusicSong*> MusicSongList::getSongList()
{
    return songList_;
}
