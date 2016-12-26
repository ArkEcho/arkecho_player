#ifndef ARKECHOPLAYERVIEW_H
#define ARKECHOPLAYERVIEW_H

#include <QtWidgets/QMainWindow>

#include "ui_ArkEchoPlayerView.h"

class ArkEchoPlayerModel;

class ArkEchoPlayerView : public QMainWindow
{
    Q_OBJECT

public:
    ArkEchoPlayerView(QWidget *parent = 0);
    ~ArkEchoPlayerView();

private:
    Ui::ArkEchoPlayerViewClass* ui_;
    ArkEchoPlayerModel* model_;

    void initUi();

private slots :
    void onUpdateView(const int &uve);
    void on_actionManuelle_Verbindung_triggered();
    void on_actionQR_Code_Verbindung_triggered();
};

#endif // ARKECHOPLAYERVIEW_H
