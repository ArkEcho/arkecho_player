#include "ArkEchoSettingsView.h"
#include "ArkEchoSettingsModel.h"

const QString DIALOGTITLE = "Einstellungen";

enum SWSETTINGSPAGEINDEX
{
    PATH = 0,
    TEST = 1
};

ArkEchoSettingsView::ArkEchoSettingsView(QWidget *parent)
    : QDialog(parent)
    ,ui_(0)
    ,model_(0)
{
    ui_ = new Ui::ArkEchoSettingsView();
    ui_->setupUi(this);
    connect(ui_->lwSettings, SIGNAL(itemPressed(QListWidgetItem*)), this, SLOT(onLwSettingsItemPressed(QListWidgetItem*)));

    model_ = new ArkEchoSettingsModel();

    initUi();
}

ArkEchoSettingsView::~ArkEchoSettingsView()
{
    delete model_;
    delete ui_;
}

void ArkEchoSettingsView::initUi()
{
    this->setWindowTitle(DIALOGTITLE);

    // ListWidget initilisieren
    QListWidgetItem* item0 = new QListWidgetItem("Dateipfade anpassen", ui_->lwSettings, SWSETTINGSPAGEINDEX::PATH);
    QListWidgetItem* item1 = new QListWidgetItem("Test", ui_->lwSettings, SWSETTINGSPAGEINDEX::TEST);

    // Set selected Page & Item
    ui_->lwSettings->setItemSelected(item0, true);
    ui_->swSettings->setCurrentIndex(0);
}

void ArkEchoSettingsView::setSwSettingsPageIndex(int index)
{
    ui_->swSettings->setCurrentIndex(index);
}

void ArkEchoSettingsView::onLwSettingsItemPressed(QListWidgetItem* item)
{
    if (!item) return;
    setSwSettingsPageIndex(item->type());
}