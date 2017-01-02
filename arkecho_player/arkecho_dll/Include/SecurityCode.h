#ifndef SECURITYCODE_H
#define SECURITYCODE_H

#include "arkecho_dll_global.h"

#include <QObject>

class ARKECHO_DLL_EXPORT SecurityCode : public QObject
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
