#include "ArkEchoPlayerView.h"

#include <QtWidgets/QApplication>
#include <QFile>
#include <QTextStream>

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);

    // Stylesheet laden
    QFile stylesheet("./resources/qdarkstyle/style.qss");
    if (stylesheet.exists())
    {
        stylesheet.open(QFile::ReadOnly | QFile::Text);
        QTextStream textstream(&stylesheet);
        application.setStyleSheet(textstream.readAll());
    }

    ArkEchoPlayerView dialog;
    dialog.show();
    return application.exec();
}
