#ifndef ARKECHOPLAYERVIEW_H
#define ARKECHOPLAYERVIEW_H

#include <QtWidgets/QMainWindow>
#include "ui_ArkEchoPlayerView.h"

class ArkEchoPlayerView : public QMainWindow
{
    Q_OBJECT

public:
    ArkEchoPlayerView(QWidget *parent = 0);
    ~ArkEchoPlayerView();

private:
    Ui::ArkEchoPlayerViewClass ui;
};

#endif // ARKECHOPLAYERVIEW_H
