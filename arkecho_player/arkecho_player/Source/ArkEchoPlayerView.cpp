#include "ArkEchoPlayerView.h"
#include "ArkEchoPlayerModel.h"
#include "ArkEchoQr.h"

const QString DIALOGTITLE = "ArkEcho Media Player";

ArkEchoPlayerView::ArkEchoPlayerView(QWidget *parent)
    : QMainWindow(parent)
{
    ui_ = new Ui::ArkEchoPlayerViewClass();
    ui_->setupUi(this);

    initUi();

    model_ = new ArkEchoPlayerModel();
    connect(model_, SIGNAL(updateView()), this, SLOT(onUpdateView()));
}

ArkEchoPlayerView::~ArkEchoPlayerView()
{
    delete model_;
    delete ui_;
}

void ArkEchoPlayerView::initUi()
{
    this->setWindowTitle(DIALOGTITLE);
}

void ArkEchoPlayerView::onUpdateView(const int &uve)
{    
}

void ArkEchoPlayerView::on_actionManuelle_Verbindung_triggered()
{
}

void ArkEchoPlayerView::on_actionQR_Code_Verbindung_triggered()
{
    ArkEchoQr* qrDialog = new ArkEchoQr("{ \"Address\": \"192.168.178.20:1000\", \"Security_Code\": 12345678 }");
    qrDialog->show();
}
