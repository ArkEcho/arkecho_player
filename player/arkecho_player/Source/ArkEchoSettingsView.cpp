#include "ArkEchoSettingsView.h"
#include "ArkEchoSettingsModel.h"

const QString DIALOGTITLE = "Einstellungen";

enum SWSETTINGSPAGEINDEX
{
    DIRECTORIES = 0,
    FORMATS = 1
    //TEST = 1
};

ArkEchoSettingsView::ArkEchoSettingsView(QWidget *parent)
    : QDialog(parent)
    ,ui_(0)
    ,model_(0)
{
    ui_ = new Ui::ArkEchoSettingsView();
    ui_->setupUi(this);
    connect(ui_->lwSettings, SIGNAL(itemPressed(QListWidgetItem*)), this, SLOT(onLwSettingsItemPressed(QListWidgetItem*)));
    connect(ui_->pbDirectoryNew, SIGNAL(clicked()), this, SLOT(onPbDirectoryNewClicked()));
    connect(ui_->pbDirectoryEdit, SIGNAL(clicked()), this, SLOT(onPbDirectoryEditClicked()));
    connect(ui_->pbDirectoryDelete, SIGNAL(clicked()), this, SLOT(onPbDirectoryDeleteClicked()));
    connect(ui_->pbFormatNew, SIGNAL(clicked()), this, SLOT(onPbFormatNewClicked()));
    connect(ui_->pbFormatEdit, SIGNAL(clicked()), this, SLOT(onPbFormatEditClicked()));
    connect(ui_->pbFormatDelete, SIGNAL(clicked()), this, SLOT(onPbFormatDeleteClicked()));

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

    // ListWidget Settings initilisieren
    QListWidgetItem* item0 = new QListWidgetItem("Dateipfade anpassen", ui_->lwSettings, SWSETTINGSPAGEINDEX::DIRECTORIES);
    QListWidgetItem* item1 = new QListWidgetItem("Musik Formate anpassen", ui_->lwSettings, SWSETTINGSPAGEINDEX::FORMATS);

    // Set selected Page & Item
    ui_->lwSettings->setItemSelected(item0, true);
    ui_->swSettings->setCurrentIndex(0);

    // Path Page
    setLwDirectories();
    setLwFormats();
}

void ArkEchoSettingsView::onPbDirectoryNewClicked()
{
    if (!model_) return;
    model_->newDirectory();
    setLwDirectories();
}

void ArkEchoSettingsView::onPbDirectoryEditClicked()
{
    if (!model_) return;
    model_->editDirectory(getLwDirectoriesSelectedItemType());
    setLwDirectories();
}

void ArkEchoSettingsView::onPbDirectoryDeleteClicked()
{
    if (!model_) return;
    model_->deleteDirectory(getLwDirectoriesSelectedItemType());
    setLwDirectories();
}

void ArkEchoSettingsView::onPbFormatNewClicked()
{
    if (!model_) return;
    model_->newFormat();
    setLwFormats();
}

void ArkEchoSettingsView::onPbFormatEditClicked()
{
    if (!model_) return;
    model_->editFormat(getLwFormatsSelectedItemType());
    setLwFormats();
}

void ArkEchoSettingsView::onPbFormatDeleteClicked()
{
    if (!model_) return;
    model_->deleteFormat(getLwFormatsSelectedItemType());
    setLwFormats();
}

void ArkEchoSettingsView::setSwSettingsPageIndex(int index)
{
    ui_->swSettings->setCurrentIndex(index);
}

void ArkEchoSettingsView::setLwDirectories()
{
    if (!model_ || !ui_) return;
    ui_->lwDirectories->clear();
    QStringList directories = model_->getDirectoriesList();
    for (int i = 0; i < directories.size(); ++i)
    {
        QListWidgetItem* item = new QListWidgetItem(directories.at(i), ui_->lwDirectories, i);
    }
}

int ArkEchoSettingsView::getLwDirectoriesSelectedItemType()
{
    if (!ui_) return -1;
    // Get selected Item (Single Selection Mode; only One)
    QListWidgetItem* item = ui_->lwDirectories->selectedItems().at(0);
    if (!item) return -1;
    return item->type();
}

void ArkEchoSettingsView::setLwFormats()
{
    if (!model_ || !ui_) return;
    ui_->lwFormats->clear();
    QStringList formats = model_->getFormatsList();
    for (int i = 0; i < formats.size(); ++i)
    {
        QListWidgetItem* item = new QListWidgetItem(formats.at(i), ui_->lwFormats, i);
    }
}

int ArkEchoSettingsView::getLwFormatsSelectedItemType()
{
    if (!ui_) return -1;
    // Get selected Item (Single Selection Mode; only One)
    QListWidgetItem* item = ui_->lwFormats->selectedItems().at(0);
    if (!item) return -1;
    return item->type();
}

void ArkEchoSettingsView::onLwSettingsItemPressed(QListWidgetItem* item)
{
    if (!item) return;
    setSwSettingsPageIndex(item->type());
}