#ifndef SECURITYCODE_H
#define SECURITYCODE_H

#include <QObject>

class SecurityCode : public QObject
{
    Q_OBJECT

public:
    SecurityCode(QObject *parent = 0);
    ~SecurityCode();

    int getSecurityCode();

private:
    int securityCode_;
};

#endif // SECURITYCODE_H
