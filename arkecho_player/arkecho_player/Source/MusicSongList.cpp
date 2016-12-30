#include "MusicSongList.h"
#include "MusicSong.h"

#include <QDirIterator>
#include <QListIterator>

MusicSongList::MusicSongList(QStringList directories, QObject *parent)
    : QObject(parent)
{
    for (int i = 0; i < directories.size(); ++i)
    {
        QDirIterator* it = new QDirIterator(directories.at(i), QStringList() << "*.mp3", QDir::Files, QDirIterator::Subdirectories);
        while (it->hasNext())
        {
            QString path = it->next();
            MusicSong* s = new MusicSong(QUrl::fromLocalFile(path));
            songList_.append(s);
        }
        delete it;
    }
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
