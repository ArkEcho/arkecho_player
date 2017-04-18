#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include "arkecho_dll_global.h"

#include <QObject>

class ARKECHO_DLL_EXPORT MessageHandler : public QObject
{
    Q_OBJECT

    MessageHandler() {}
    ~MessageHandler() {}

public:
    enum MESSAGETYPE
    {
        MT_NOTDEFINED = 0,
        MT_ECHO_TEST,
        MT_PLAY_PAUSE,
        MT_BACKWARD,
        MT_FORWARD,
        MT_REQUEST_SONG_ACTUAL,
        MT_SEND_SONG_ACTUAL,
        MT_REQUEST_SONGLIST,
        MT_SEND_SONGLIST,
        MT_SHUFFLE,
        MT_STOP,
        MT_VOLUME_VALUE
    };

    // Setzt die Nachricht und den Typ zum JSON zusammenzimmern
    static QString& createMessage(int typ, QString& message);

    // Eingegebene Nachricht aus JSON auseinenanderzimmern; Nachrichtentyp als @return
    static int handleReceivedMessage(QString &message);
};

#endif // MESSAGEHANDLER_H
