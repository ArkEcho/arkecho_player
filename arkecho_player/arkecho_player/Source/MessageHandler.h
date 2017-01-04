#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include <QObject>

enum MESSAGETYPE
{
    MT_NOTDEFINED = 0,
    MT_ECHO_TEST,
    MT_PLAY_PAUSE,
    MT_BACKWARD,
    MT_FORWARD
};

class MessageHandler : public QObject
{
    Q_OBJECT

    MessageHandler() {}
    ~MessageHandler() {}

public:
    // Setzt die Nachricht und den Typ zum JSON zusammenzimmern
    static QString& createMessage(int typ, QString& message);

    // Eingegebene Nachricht aus JSON auseinenanderzimmern; Nachrichtentyp als @return
    static int handleReceivedMessage(QString &message);
};

#endif // MESSAGEHANDLER_H
