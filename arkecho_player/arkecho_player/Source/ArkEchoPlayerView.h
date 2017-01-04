#ifndef ARKECHOPLAYERVIEW_H
#define ARKECHOPLAYERVIEW_H

#include <QMainWindow>
#include <QMediaPlayer>

#include "ui_ArkEchoPlayerView.h"

class ArkEchoPlayerModel;
class QLabel;

class ArkEchoPlayerView : public QMainWindow
{
    Q_OBJECT

    Ui::ArkEchoPlayerViewClass* ui_;
    ArkEchoPlayerModel* model_;
    QLabel* webSocketStatus_;
    QMediaPlayer* player_;

    void initUi();
    void setWebSocketStatusLabel(bool connected);
    void setTWTrackList(QString filterText = "");
    void setLblDuration();
    void setActualSongInfoCoverArt(QImage image);
    void setActualSongInfoText();

public:
    ArkEchoPlayerView(QWidget *parent = 0);
    ~ArkEchoPlayerView();

private slots :
    void onUpdateView(const int &uve);
    void on_actionManuelle_Verbindung_triggered();
    void on_actionQR_Code_Verbindung_triggered();
    void onPbBackwardClicked();
    void onPbForwardClicked();
    void onPbPlay_PauseClicked();
    void onPbStopClicked();
    void onTwTrackListItemDoubleClicked(QTableWidgetItem* item);
    void onSliderVolumeValueChanged(const int &value);
    void onPlayerPositionChanged(const qint64 &position);
    void onSliderDurationPressed();
    void onSliderDurationReleased();
    void onLeFilterTextChanged(const QString& text);
    void onPbClearFilterClicked();
    void onPbShuffleClicked();
    void onPlayerMediaStatusChanged(const QMediaPlayer::MediaStatus& status);
};

#endif // ARKECHOPLAYERVIEW_H
