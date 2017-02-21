#ifndef ARKECHOSETTINGSMODEL_H
#define ARKECHOSETTINGSMODEL_H

#include <QObject>

class ArkEchoSettingsModel : public QObject
{
    Q_OBJECT

    // Temporäre Kopie der Music Directories List
    QStringList directoriesList_;
    QStringList formatsList_;

public:
    ArkEchoSettingsModel(QObject *parent = 0);
    ~ArkEchoSettingsModel();

    QStringList& getDirectoriesList();
    QStringList& getFormatsList();
    void newDirectory();
    void editDirectory(int index);
    void deleteDirectory(int index);
    void newFormat();
    void editFormat(int index);
    void deleteFormat(int index);
};

#endif // ARKECHOSETTINGSMODEL_H
