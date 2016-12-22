#include "ArkEchoPlayerView.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ArkEchoPlayerView w;
    w.show();
    return a.exec();
}
