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

public:
    ArkEchoSettingsView(QWidget *parent = 0);
    ~ArkEchoSettingsView();

private slots:
    void onLwSettingsItemPressed(QListWidgetItem* item);
};

#endif // ARKECHOSETTINGSVIEW_H
