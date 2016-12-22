#ifndef ARKECHOPLAYERMODEL_H
#define ARKECHOPLAYERMODEL_H

#include <QObject>

enum UpdateViewEnum
{
    TEST = 0
};

class ArkEchoPlayerModel : public QObject
{
    Q_OBJECT

public:
    ArkEchoPlayerModel(QObject *parent = 0);
    ~ArkEchoPlayerModel();

signals:
    void updateView(int);

private:
    
};

#endif // ARKECHOPLAYERMODEL_H
