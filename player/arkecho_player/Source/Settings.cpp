#include "Settings.h"

#include <QDir>
#include <QTextStream>

const QString FILENAME = "settings.ark";
const QString KEY_MUSIC_DIRECTORIES = "#Music Directories";
const QString KEY_MUSIC_FORMATS = "#Music Formats";

QStringList Settings::musicDirectoriesList_;
QStringList Settings::musicFormatsList_;
Settings::Constructor Settings::cons_;

void Settings::loadSettingsFromFile()
{
    QFile file(FILENAME);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) // Default Settings if File doesn't exist
    {
        musicDirectoriesList_.append(QDir::homePath() + "/Music/");
        musicFormatsList_ << "*.mp3" << "*.m4a" << "*.wma";
    }

    QTextStream in(&file);
    QString line = "";  
    QString key = "";
    while (true)
    {
        line = in.readLine();
        if (line == "") break;
        else if (line.startsWith("#"))
        {
            key = line;
            continue;
        }
        else if (key == KEY_MUSIC_DIRECTORIES) musicDirectoriesList_.append(line);
        else if (key == KEY_MUSIC_FORMATS) musicFormatsList_.append(line);
    }

    file.close();
    file.remove();
}

void Settings::saveSettingsToFile()
{
    QFile file(FILENAME);
    file.remove();

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return;

    QTextStream out(&file);

    // Output of Music Directories List
    out << KEY_MUSIC_DIRECTORIES << endl;
    for (int i = 0; i < musicDirectoriesList_.size(); ++i)
    {
        out << musicDirectoriesList_.at(i) << endl;
    }

    // Output of Music Formats List
    out << KEY_MUSIC_FORMATS << endl;
    for (int i = 0; i < musicFormatsList_.size(); ++i)
    {
        out << musicFormatsList_.at(i) << endl;
    }

    file.close();
}

QStringList & Settings::getMusicDirectoriesList()
{
    return musicDirectoriesList_;
}

void Settings::setMusicDirectoriesList(QStringList & list)
{
    musicDirectoriesList_ = list;
}

QStringList & Settings::getMusicFormatsList()
{
    return musicFormatsList_;
}

void Settings::setMusicFormatsList(QStringList & list)
{
    musicFormatsList_ = list;
}
