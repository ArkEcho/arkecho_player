#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include <QObject>

class MessageHandler : public QObject
{
    Q_OBJECT

public:
    enum MESSAGETYPE
    {
        NOTDEFINED = 0,
        ECHO_TEST,
        HANDSHAKE_SEC_CODE
    };

    // Setzt die Nachricht und den Typ zum JSON zusammenzimmern
    static QString& createMessage(int typ, QString& message);

    // Eingegebene Nachricht aus JSON auseinenanderzimmern; Nachrichtentyp als @return
    static int handleReceivedMessage(QString &message);

private:
    MessageHandler() {}
    ~MessageHandler() {}
};

#endif // MESSAGEHANDLER_H
