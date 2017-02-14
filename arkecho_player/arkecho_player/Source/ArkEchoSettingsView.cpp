#include "ArkEchoSettingsView.h"
#include "ArkEchoSettingsModel.h"

ArkEchoSettingsView::ArkEchoSettingsView(QWidget *parent)
    : QDialog(parent)
    ,ui_(0)
    ,model_(0)
{
    ui_ = new Ui::ArkEchoSettingsView();
    ui_->setupUi(this);

    model_ = new ArkEchoSettingsModel();
}

ArkEchoSettingsView::~ArkEchoSettingsView()
{
    delete model_;
    delete ui_;
}

