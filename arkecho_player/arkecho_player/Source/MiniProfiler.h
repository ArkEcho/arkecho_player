#ifndef MINIPROFILER_H
#define MINIPROFILER_H

#include <QString>
#include <QElapsedTimer>
#include <QDebug>

class MiniProfiler
{
public:
    MiniProfiler(QString name);
    inline void start()
    {
        qDebug() <<qPrintable(name)<<" gestartet";
        elapsed.start();
    }
    inline void stop()
    {
        qDebug() <<qPrintable(name) << " lief " << elapsed.nsecsElapsed()*0.000001 << " Millisekunden";
    }

private:
    QString name;
    QElapsedTimer elapsed;
};

#endif // MINIPROFILER_H
