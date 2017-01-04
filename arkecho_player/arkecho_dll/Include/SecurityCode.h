#ifndef SECURITYCODE_H
#define SECURITYCODE_H

#include "arkecho_dll_global.h"

#include <QObject>

class ARKECHO_DLL_EXPORT SecurityCode : public QObject
{
    Q_OBJECT

    int securityCode_;

public:
    SecurityCode(QObject *parent = 0);
    ~SecurityCode();

    int getSecurityCode();
};

#endif // SECURITYCODE_H
