#ifndef ARKECHOSETTINGSVIEW_H
#define ARKECHOSETTINGSVIEW_H

#include <QDialog>
#include "ui_ArkEchoSettingsView.h"

class ArkEchoSettingsModel;

class ArkEchoSettingsView : public QDialog
{
    Q_OBJECT

    Ui::ArkEchoSettingsView *ui_;
    ArkEchoSettingsModel* model_;
    
    void initUi();
    void setSwSettingsPageIndex(int index);

    void setLwDirectories();
    int getLwDirectoriesSelectedItemType();

public:
    ArkEchoSettingsView(QWidget *parent = 0);
    ~ArkEchoSettingsView();

private slots:
    void onLwSettingsItemPressed(QListWidgetItem* item);
    void onPbDirectoryNewClicked();
    void onPbDirectoryEditClicked();
    void onPbDirectoryDeleteClicked();
};

#endif // ARKECHOSETTINGSVIEW_H
