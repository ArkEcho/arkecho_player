#include "SecurityCode.h"

#include <QDateTime>

SecurityCode::SecurityCode(QObject *parent)
    : QObject(parent)
{
    qint64 ms = QDateTime::currentDateTime().toMSecsSinceEpoch();
    srand(ms);
    int random = rand() % 9999 + 1000;
    if (random > 9999) random /= 10;
    securityCode_ = random;
}

SecurityCode::~SecurityCode()
{

}

int SecurityCode::getSecurityCode()
{
    return securityCode_;
}
