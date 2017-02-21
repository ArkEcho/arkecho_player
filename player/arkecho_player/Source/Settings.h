#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>

class Settings : public QObject
{
    Q_OBJECT
    Settings(QObject *parent = 0) :QObject(parent) {};
    ~Settings() {};
    friend class Constructor;
    struct Constructor {
        Constructor() { loadSettingsFromFile(); }
        ~Constructor() { saveSettingsToFile(); }
    };
    static Constructor cons_;

    static void loadSettingsFromFile();
    static void saveSettingsToFile();

    static QStringList musicDirectoriesList_;
    static QStringList musicFormatsList_;

public:
    static QStringList& getMusicDirectoriesList();
    static void setMusicDirectoriesList(QStringList& list);

    static QStringList& getMusicFormatsList();
    static void setMusicFormatsList(QStringList& list);
};

#endif // SETTINGS_H
