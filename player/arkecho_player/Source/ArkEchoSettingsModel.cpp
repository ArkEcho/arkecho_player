#include "ArkEchoSettingsModel.h"
#include "Settings.h"

#include <QFileDialog>

ArkEchoSettingsModel::ArkEchoSettingsModel(QObject *parent)
    : QObject(parent)
{
    directoriesList_ = Settings::getMusicDirectoriesList();
}

ArkEchoSettingsModel::~ArkEchoSettingsModel()
{
    Settings::setMusicDirectoriesList(directoriesList_);
}

QStringList & ArkEchoSettingsModel::getDirectoriesList()
{
    return directoriesList_;
}

void ArkEchoSettingsModel::newDirectory()
{
    QString dir = QFileDialog::getExistingDirectory(0,tr("Neuer Ordner:"), "/home",
                        QFileDialog::ShowDirsOnly| QFileDialog::DontResolveSymlinks);
    directoriesList_.append(dir);
}

void ArkEchoSettingsModel::editDirectory(int index)
{
    QString actual = directoriesList_.at(index);
    QString dir = QFileDialog::getExistingDirectory(0, tr("Ordner anpassen:"), actual,
                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    directoriesList_.replace(index, dir);
}

void ArkEchoSettingsModel::deleteDirectory(int index)
{
    directoriesList_.removeAt(index);
}
