#ifndef ARKECHOSETTINGSMODEL_H
#define ARKECHOSETTINGSMODEL_H

#include <QObject>

class ArkEchoSettingsModel : public QObject
{
    Q_OBJECT

public:
    ArkEchoSettingsModel(QObject *parent = 0);
    ~ArkEchoSettingsModel();
};

#endif // ARKECHOSETTINGSMODEL_H
