#include "ArkEchoPlayerView.h"
#include "ArkEchoPlayerModel.h"

ArkEchoPlayerView::ArkEchoPlayerView(QWidget *parent)
    : QMainWindow(parent)
{
    ui_ = new Ui::ArkEchoPlayerViewClass();
    ui_->setupUi(this);

    model_ = new ArkEchoPlayerModel();
    connect(model_, SIGNAL(updateView()), this, SLOT(onUpdateView()));
}

ArkEchoPlayerView::~ArkEchoPlayerView()
{
    delete model_;
    delete ui_;
}

void ArkEchoPlayerView::onUpdateView(const int &uve)
{
    
}
