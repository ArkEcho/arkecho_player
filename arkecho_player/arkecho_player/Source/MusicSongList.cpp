#include "MusicSongList.h"

#include <QDirIterator>
#include <QListIterator>

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
    QListIterator<MusicSong*> it(songList_);
    while (it.hasNext())
    {
        MusicSong* s = it.next();
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
            songList_.append(s);
        }
    }
    delete it;
}

QList<MusicSong*> MusicSongList::getSongList()
{
    return songList_;
}
