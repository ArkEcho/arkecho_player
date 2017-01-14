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
    if (songList_.size() == 0) return true;
    int countNotLoaded = 0;
    QMapIterator<int,MusicSong*> it(songList_);
    while (it.hasNext())
    {
        int key = it.next().key();
        MusicSong* s = songList_[key];
        if (!s) continue;
        if (s->getStatus() == QMediaPlayer::MediaStatus::InvalidMedia)
        {
            delete s;
            songList_.remove(key);
        }
        else if (s->getStatus() != QMediaPlayer::MediaStatus::LoadedMedia)
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

QMap<int,MusicSong*> MusicSongList::getSongList()
{
    return songList_;
}
