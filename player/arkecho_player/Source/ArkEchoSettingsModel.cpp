#include "ArkEchoSettingsModel.h"
#include "Settings.h"

#include <QFileDialog>
#include <QInputDialog>

ArkEchoSettingsModel::ArkEchoSettingsModel(QObject *parent)
    : QObject(parent)
{
    directoriesList_ = Settings::getMusicDirectoriesList();
    formatsList_ = Settings::getMusicFormatsList();
}

ArkEchoSettingsModel::~ArkEchoSettingsModel()
{
    Settings::setMusicDirectoriesList(directoriesList_);
    Settings::setMusicFormatsList(formatsList_);
}

QStringList & ArkEchoSettingsModel::getDirectoriesList()
{
    return directoriesList_;
}

QStringList & ArkEchoSettingsModel::getFormatsList()
{
    return formatsList_;
}

void ArkEchoSettingsModel::newDirectory()
{
    QString dir = QFileDialog::getExistingDirectory(0,tr("Neuer Ordner:"), "/home",
                        QFileDialog::ShowDirsOnly| QFileDialog::DontResolveSymlinks);
    directoriesList_.append(dir);
}

void ArkEchoSettingsModel::editDirectory(int index)
{
    if (index < 0) return;
    QString actual = directoriesList_.at(index);
    QString dir = QFileDialog::getExistingDirectory(0, tr("Ordner anpassen:"), actual,
                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    directoriesList_.replace(index, dir);
}

void ArkEchoSettingsModel::deleteDirectory(int index)
{
    if (index < 0) return;
    directoriesList_.removeAt(index);
}

void ArkEchoSettingsModel::newFormat()
{
    QString format = QInputDialog::getText(0, tr("Neues Format:"), "Format:");
    formatsList_.append(format);
}

void ArkEchoSettingsModel::editFormat(int index)
{
    if (index < 0) return;
    QString actual = formatsList_.at(index);
    QString format = QInputDialog::getText(0, tr("Neues Format:"), "Format:", QLineEdit::Normal, actual);
    formatsList_.replace(index, format);
}

void ArkEchoSettingsModel::deleteFormat(int index)
{
    if (index < 0) return;
    formatsList_.removeAt(index);
}
