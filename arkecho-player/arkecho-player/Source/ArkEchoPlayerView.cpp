#include "ArkEchoPlayerView.h"
#include "ArkEchoPlayerModel.h"

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
