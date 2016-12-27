#include "ArkEchoPlayerView.h"
#include "ArkEchoPlayerModel.h"

#include <QLabel>

const QString DIALOGTITLE = "ArkEcho Media Player";

ArkEchoPlayerView::ArkEchoPlayerView(QWidget *parent)
    : QMainWindow(parent)
{
    ui_ = new Ui::ArkEchoPlayerViewClass();
    ui_->setupUi(this);

    webSocketStatus_ = new QLabel();

    // UI Anzeige initialisieren
    initUi();
    
    setWebSocketStatusLabel(false);

    model_ = new ArkEchoPlayerModel();
    connect(model_, SIGNAL(updateView(int)), this, SLOT(onUpdateView(int)));
}

ArkEchoPlayerView::~ArkEchoPlayerView()
{
    delete model_;
    delete ui_;
}

void ArkEchoPlayerView::initUi()
{
    this->setWindowTitle(DIALOGTITLE);

    // WebSocket Statusanzeige initalisieren
    ui_->statusBar->addPermanentWidget(webSocketStatus_);
}

void ArkEchoPlayerView::setWebSocketStatusLabel(bool connected)
{
    if (!webSocketStatus_) return;
    QString message, color;
    if (connected)
    {
        message = "Verbunden!";
        color = "rgb(24,220,44)";
    }
    else
    {
        message = "Nicht Verbunden!";
        color = "red";
    }
    webSocketStatus_->setText(message);
    webSocketStatus_->setStyleSheet("color: "+color+";");
}

void ArkEchoPlayerView::onUpdateView(const int &uve)
{
    switch(uve)
    {
    case UVE_WEBSOCKET_CONNECTED:
        setWebSocketStatusLabel(true);
        break;
    case UVE_WEBSOCKET_DISCONNECTED:
        setWebSocketStatusLabel(false);
        break;
    }
    qApp->processEvents();
}

void ArkEchoPlayerView::on_actionManuelle_Verbindung_triggered()
{
    model_->showConnectManualDialog();
}

void ArkEchoPlayerView::on_actionQR_Code_Verbindung_triggered()
{
    model_->showConnectQrDialog();
}
