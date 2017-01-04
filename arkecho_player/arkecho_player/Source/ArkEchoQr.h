#ifndef ARKECHOQR_H
#define ARKECHOQR_H

#include "ui_ArkEchoQr.h"

#include <QDialog>

class ArkEchoQr : public QDialog
{
    Q_OBJECT

    Ui::ArkEchoQr* ui_;
    QString qrCodeText_;
    QImage* qrCode_;

    void createQrCode(int scaleSize);
    void setQrCodePixel(int x, int y, int color, int scale = 1);

public:
    ArkEchoQr(QString qrCodeText, QWidget *parent = 0);
    ~ArkEchoQr();
};

#endif // ARKECHOQR_H
