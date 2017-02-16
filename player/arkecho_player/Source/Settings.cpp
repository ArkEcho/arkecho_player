#include "Settings.h"

#include <QDir>
#include <QTextStream>

const QString FILENAME = "settings.ark";

QStringList Settings::musicDirectoriesList_;
Settings::Constructor Settings::cons_;

void Settings::loadSettingsFromFile()
{
    QFile file(FILENAME);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) // Default Settings if File doesn't exist
    {
        musicDirectoriesList_ = QStringList() << QDir::homePath() + "/Music/";
    }

    QTextStream in(&file);
    QString line = "";    
    while (true)
    {
        line = in.readLine();
        if (line == "") break;
        else if (line.startsWith("#")) continue;
        else if (line != "") musicDirectoriesList_ = QStringList() << line;
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
    out << "#Music Directories" << endl;
    for (int i = 0; i < musicDirectoriesList_.size(); ++i)
    {
        out << musicDirectoriesList_.at(i) << endl;
    }

    file.close();
}

QStringList & Settings::getMusicDirectoriesList()
{
    return musicDirectoriesList_;
}
