#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include <QObject>

enum MESSAGETYPE
{
    MT_NOTDEFINED = 0,
    MT_ECHO_TEST,
    MT_HANDSHAKE_SEC_CODE
};

class MessageHandler : public QObject
{
    Q_OBJECT

public:

    // Setzt die Nachricht und den Typ zum JSON zusammenzimmern
    static QString& createMessage(int typ, QString& message);

    // Eingegebene Nachricht aus JSON auseinenanderzimmern; Nachrichtentyp als @return
    static int handleReceivedMessage(QString &message);

private:
    MessageHandler() {}
    ~MessageHandler() {}
};

#endif // MESSAGEHANDLER_H
