#ifndef LIBRARY_H
#define LIBRARY_H

#include "MusicSongList.h"
#include <QObject>

class Library : public QObject
{
    Q_OBJECT

    Library(QObject *parent);
    ~Library();
    friend class Constructor;
    struct Constructor {
        Constructor() { }
        ~Constructor() { }
    };
    static Constructor cons_;
    
public:

};

#endif // LIBRARY_H
