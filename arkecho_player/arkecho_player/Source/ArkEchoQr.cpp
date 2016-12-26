#include "ArkEchoQr.h"
#include "QRCodeGenerator\QrCode.hpp"

#include <QImage>

using namespace qrcodegen;

const int RGB_WHITE = 255 * 65536 + 255 * 256 + 255;
const int RGB_BLACK = 0;

ArkEchoQr::ArkEchoQr(QString qrCodeText, QWidget *parent)
    : QDialog(parent)
    , qrCodeText_(qrCodeText)
    , qrCode_(0)
{
    ui_ = new Ui::ArkEchoQr();
    ui_->setupUi(this);
    this->setStyleSheet("background-color: white;");
    this->setMinimumSize(200, 200);
    this->setMaximumSize(200, 200);

    createQrCode(3.5);
    ui_->lblQrCode->setPixmap(QPixmap::fromImage(*qrCode_));
}

ArkEchoQr::~ArkEchoQr()
{
    delete qrCode_;
    delete ui_;
}

void ArkEchoQr::createQrCode(int scaleSize)
{
    QrCode qr = QrCode::encodeText(qrCodeText_.toUtf8(), QrCode::Ecc::QUARTILE);

    int imageSize = qr.size*scaleSize;
    qrCode_ = new QImage(imageSize, imageSize, QImage::Format::Format_RGB32);

    // Read the black & white pixels
    int color;
    for (int x = 0; x < qr.size; ++x) {
        for (int y = 0; y < qr.size; ++y) {
            color = qr.getModule(x, y);  // 0 for white, 1 for black
            if (color == 0)
            {
                setQrCodePixel(x, y, RGB_WHITE, scaleSize);
            }
            if (color == 1)
            {
                setQrCodePixel(x, y, RGB_BLACK, scaleSize);
            }
        }
    }
}

void ArkEchoQr::setQrCodePixel(int x, int y, int color, int scale)
{
    if (scale < 1) return;

    x = (x * scale) - 1;
    y = (y * scale) - 1;

    for (int iX = 1; iX <= scale; ++iX)
    {
        for (int iY = 1; iY <= scale; ++iY)
        {
            qrCode_->setPixel(x+iX, y+iY, color);
        }
    }
}

