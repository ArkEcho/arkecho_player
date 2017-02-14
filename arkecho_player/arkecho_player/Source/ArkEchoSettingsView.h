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

public:
    ArkEchoSettingsView(QWidget *parent = 0);
    ~ArkEchoSettingsView();
};

#endif // ARKECHOSETTINGSVIEW_H
