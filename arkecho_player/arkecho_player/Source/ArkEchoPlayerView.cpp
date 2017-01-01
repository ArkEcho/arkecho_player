#include "ArkEchoPlayerView.h"
#include "ArkEchoPlayerModel.h"
#include "MusicSongList.h"

#include <QLabel>

const QString DIALOGTITLE = "ArkEcho Media Player";
const int DEFAULT_VOLUME = 50;

enum TableTrackListColumns
{
    TRACKL_ALBUMTITLE = 0,
    TRACKL_ALBUMNUMBER,
    TRACKL_SONGTITLE,
    TRACKL_SONGINTERPRET,
    TRACKL_SONGDURATION,
    TRACKL_MAX_COLUMN_COUNT
};

ArkEchoPlayerView::ArkEchoPlayerView(QWidget *parent)
    :QMainWindow(parent)
    ,ui_(0)
    ,model_(0)
    ,webSocketStatus_(0)
    ,player_(0)
{
    model_ = new ArkEchoPlayerModel();
    connect(model_, SIGNAL(updateView(int)), this, SLOT(onUpdateView(int)));

    player_ = new QMediaPlayer();
    connect(player_, SIGNAL(positionChanged(qint64)), this, SLOT(onPlayerPositionChanged(qint64)));

    ui_ = new Ui::ArkEchoPlayerViewClass();
    ui_->setupUi(this);
    connect(ui_->pbBackward, SIGNAL(clicked()), this, SLOT(onPbBackwardClicked()));
    connect(ui_->pbForward, SIGNAL(clicked()), this, SLOT(onPbForwardClicked()));
    connect(ui_->pbPlay_Pause, SIGNAL(clicked()), this, SLOT(onPbPlay_PauseClicked()));
    connect(ui_->pbStop, SIGNAL(clicked()), this, SLOT(onPbStopClicked()));
    connect(ui_->twTrackList, SIGNAL(itemDoubleClicked(QTableWidgetItem*)), this, SLOT(onTwTrackListItemDoubleClicked(QTableWidgetItem*)));
    connect(ui_->sliderVolume, SIGNAL(valueChanged(int)), this, SLOT(onSliderVolumeValueChanged(int)));
    // UI initialisieren; Gr�ssen, Texte, Inhalt etc.
    initUi();
}

ArkEchoPlayerView::~ArkEchoPlayerView()
{
    delete webSocketStatus_;
    delete model_;
    delete player_;
    delete ui_;
}

void ArkEchoPlayerView::initUi()
{
    this->setWindowTitle(DIALOGTITLE);

    // WebSocket Statusanzeige initalisieren
    webSocketStatus_ = new QLabel();
    ui_->statusBar->addPermanentWidget(webSocketStatus_);
    setWebSocketStatusLabel(false);

    //TableWidget initialisieren
    ui_->twTrackList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui_->twTrackList->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui_->twTrackList->verticalHeader()->setVisible(false);
    ui_->twTrackList->setColumnCount(TRACKL_MAX_COLUMN_COUNT);
    ui_->twTrackList->setHorizontalHeaderLabels(QString("Album;Number;Titel;Interpret;Dauer").split(";"));
    setTWTrackList();

    //Volume initialisieren
    ui_->lblVolume->setText(QString::number(DEFAULT_VOLUME));
    ui_->sliderVolume->setValue(DEFAULT_VOLUME);
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

void ArkEchoPlayerView::setTWTrackList()
{
    if (!model_ || !ui_ || !model_->getMusicSongList()) return;

    QMap<int,MusicSong*> map = model_->getMusicSongList()->getSongList();
    int mapSize = map.size();
    if (mapSize == 0) return;
    ui_->twTrackList->setRowCount(mapSize);

    for (int i = 0; i < mapSize; ++i)
    {
        MusicSong* song = map.value(i);
        if (!song) continue;
        ui_->twTrackList->setItem(i, TRACKL_ALBUMTITLE, new QTableWidgetItem(song->getAlbumTitle(), i));
        ui_->twTrackList->setItem(i, TRACKL_ALBUMNUMBER, new QTableWidgetItem(QString::number(song->getAlbumSongNumber()), i));
        ui_->twTrackList->setItem(i, TRACKL_SONGTITLE, new QTableWidgetItem(song->getSongTitle(), i));
        ui_->twTrackList->setItem(i, TRACKL_SONGINTERPRET, new QTableWidgetItem(song->getSongInterpret(), i));
        ui_->twTrackList->setItem(i, TRACKL_SONGDURATION, new QTableWidgetItem(song->getSongDurationAsMinuteSecond(), i));
    }
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
    case REMOTE_BUTTON_BACKWARD:
        onPbPlay_PauseClicked();
        break;
    case REMOTE_BUTTON_FORWARD:
        onPbPlay_PauseClicked();
        break;
    case REMOTE_BUTTON_PLAY_PAUSE:
        onPbPlay_PauseClicked();
        break;
    }
    qApp->processEvents();
}

void ArkEchoPlayerView::on_actionManuelle_Verbindung_triggered()
{
    if (!model_) return;
    model_->showConnectManualDialog();
}

void ArkEchoPlayerView::on_actionQR_Code_Verbindung_triggered()
{
    if (!model_) return;
    model_->showConnectQrDialog();
}

void ArkEchoPlayerView::onPbBackwardClicked()
{
}

void ArkEchoPlayerView::onPbForwardClicked()
{
}

void ArkEchoPlayerView::onPbPlay_PauseClicked()
{
    if (!player_) return;
    if (player_->state() == QMediaPlayer::PlayingState) player_->pause();
    else player_->play();
}

void ArkEchoPlayerView::onPbStopClicked()
{
    if (!player_) return;
    player_->stop();
}

void ArkEchoPlayerView::onTwTrackListItemDoubleClicked(QTableWidgetItem * item)
{
    if (!item) return;
    int type = item->type();

    if (!model_ || !model_->getMusicSongList()) return;
    MusicSong* song = model_->getMusicSongList()->getSongList()[type];

    if (!player_ || !song) return;
    player_->setMedia(song->getUrl());
    player_->play();
}

void ArkEchoPlayerView::onSliderVolumeValueChanged(const int &value)
{
    if (!player_) return;
    player_->setVolume(value);
    ui_->lblVolume->setText(QString::number(value));
}

void ArkEchoPlayerView::onPlayerPositionChanged(const qint64 & position)
{
    if (!player_) return;
    double duration = (double)player_->duration();
    double positionD = (double)position;

    if (duration == 0) return;
    double value = (positionD / duration) * 100;
    ui_->sliderDuration->setValue(value);
}
