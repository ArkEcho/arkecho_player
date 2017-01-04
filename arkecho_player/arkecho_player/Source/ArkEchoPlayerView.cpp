#include "ArkEchoPlayerView.h"
#include "ArkEchoPlayerModel.h"
#include "MusicSongList.h"

#include <QLabel>

const QString DIALOGTITLE = "ArkEcho Media Player";
// MetaData und Player Song Länge unterscheiden sich etwa um 560ms
const int MEDIAPLAYER_BUFFER_DURATION = 560;
const int DEFAULT_VOLUME = 100;
const int ACTUAL_SONG_INFO_TEXT_WIDTH = 150;
const QSize ACTUAL_SONG_INFO_COVER_SIZE = QSize(150, 150);
const QString ACTUAL_SONG_INFO_TEXT_DEFAULT = "<Kein Lied gestartet>";
const QString ACTUAL_SONG_INFO_TEXT_EMPTY = "<Keine Sonfinfo>";
const int ROW_HEIGHT = 20;

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
    player_->setPlaylist(model_->getMediaPlaylist());
    connect(player_, SIGNAL(positionChanged(qint64)), this, SLOT(onPlayerPositionChanged(qint64)));
    connect(player_, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)), this, SLOT(onPlayerMediaStatusChanged(QMediaPlayer::MediaStatus)));

    ui_ = new Ui::ArkEchoPlayerViewClass();
    ui_->setupUi(this);
    connect(ui_->pbBackward, SIGNAL(clicked()), this, SLOT(onPbBackwardClicked()));
    connect(ui_->pbForward, SIGNAL(clicked()), this, SLOT(onPbForwardClicked()));
    connect(ui_->pbPlay_Pause, SIGNAL(clicked()), this, SLOT(onPbPlay_PauseClicked()));
    connect(ui_->pbStop, SIGNAL(clicked()), this, SLOT(onPbStopClicked()));
    connect(ui_->twTrackList, SIGNAL(itemDoubleClicked(QTableWidgetItem*)), this, SLOT(onTwTrackListItemDoubleClicked(QTableWidgetItem*)));
    connect(ui_->sliderVolume, SIGNAL(valueChanged(int)), this, SLOT(onSliderVolumeValueChanged(int)));
    connect(ui_->sliderDuration, SIGNAL(sliderPressed()), this, SLOT(onSliderDurationPressed()));
    connect(ui_->sliderDuration, SIGNAL(sliderReleased()), this, SLOT(onSliderDurationReleased()));
    connect(ui_->leFilter, SIGNAL(textChanged(QString)), this, SLOT(onLeFilterTextChanged(QString)));
    connect(ui_->pbFilterClear, SIGNAL(clicked()), this, SLOT(onPbClearFilterClicked()));
    connect(ui_->pbShuffle, SIGNAL(clicked()), this, SLOT(onPbShuffleClicked()));
    // UI initialisieren; Grössen, Texte, (Default)Inhalt etc.
    initUi();
}

ArkEchoPlayerView::~ArkEchoPlayerView()
{
    delete model_;
    delete player_;
    delete webSocketStatus_;
    delete ui_;
}

void ArkEchoPlayerView::initUi()
{
    this->setWindowTitle(DIALOGTITLE);

    // WebSocket Statusanzeige initalisieren
    webSocketStatus_ = new QLabel();
    ui_->statusBar->addPermanentWidget(webSocketStatus_);
    setWebSocketStatusLabel(false);

    // TableWidget initialisieren
    ui_->twTrackList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui_->twTrackList->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui_->twTrackList->verticalHeader()->setVisible(false);
    ui_->twTrackList->setColumnCount(TRACKL_MAX_COLUMN_COUNT);
    ui_->twTrackList->setHorizontalHeaderLabels(QString("Album;Nummer;Titel;Interpret;Dauer").split(";"));
    ui_->twTrackList->setColumnWidth(TRACKL_ALBUMTITLE, 200);
    ui_->twTrackList->setColumnWidth(TRACKL_ALBUMNUMBER, 60);
    ui_->twTrackList->setColumnWidth(TRACKL_SONGTITLE, 200);
    ui_->twTrackList->setColumnWidth(TRACKL_SONGINTERPRET, 200);
    ui_->twTrackList->setColumnWidth(TRACKL_SONGDURATION, 60);
    setTWTrackList();

    // Duration Anzeige
    ui_->lblDuration->setText("0:00");// / 0:00");

    // Volume initialisieren
    ui_->lblVolume->setText(QString::number(DEFAULT_VOLUME));
    ui_->sliderVolume->setValue(DEFAULT_VOLUME);

    // Actual Song Info initalisieren
    ui_->lblCoverArt->setScaledContents(true);
    ui_->lblCoverArt->setMinimumSize(ACTUAL_SONG_INFO_COVER_SIZE);
    ui_->lblCoverArt->setMaximumSize(ACTUAL_SONG_INFO_COVER_SIZE);
    ui_->lblSongTitle->setMaximumWidth(ACTUAL_SONG_INFO_TEXT_WIDTH);
    ui_->lblSongInterpret->setMaximumWidth(ACTUAL_SONG_INFO_TEXT_WIDTH);
    ui_->lblAlbumTitle->setMaximumWidth(ACTUAL_SONG_INFO_TEXT_WIDTH);
    ui_->lblAlbumInterpret->setMaximumWidth(ACTUAL_SONG_INFO_TEXT_WIDTH);
    setActualSongInfo(true);
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

void ArkEchoPlayerView::setTWTrackList(QString filterText)
{
    if (!model_ || !model_->getMusicSongList()) return;

    ui_->twTrackList->clearContents();
    ui_->twTrackList->setRowCount(0);

    QMap<int,MusicSong*> map = model_->getMusicSongList()->getSongList();
    int mapSize = map.size();
    if (mapSize == 0) return;

    int row = 0;
    QMapIterator<int, MusicSong*> it(map);
    while (it.hasNext())
    {
        int key = it.next().key();
        MusicSong* song = map.value(key);
        if (!song) continue;

        bool cont = true;
        if (filterText != "")
        {
            QStringList list = filterText.split(QRegExp("\\s")); // Split text by Whitespace
            QString allTogether = song->getAlbumTitle() + song->getSongTitle() + song->getSongInterpret();
            QStringListIterator it(list);
            while (it.hasNext())
            {
                QString filter = it.next();
                if (!allTogether.contains(filter, Qt::CaseInsensitive))
                {
                    cont = false;
                    break;
                }
            }
        }
        if (!cont) continue;

        ui_->twTrackList->setRowCount(row + 1);
        ui_->twTrackList->setRowHeight(row, ROW_HEIGHT);
        ui_->twTrackList->setItem(row, TRACKL_ALBUMTITLE, new QTableWidgetItem(song->getAlbumTitle(), key));
        ui_->twTrackList->setItem(row, TRACKL_ALBUMNUMBER, new QTableWidgetItem(QString::number(song->getAlbumSongNumber()), key));
        ui_->twTrackList->setItem(row, TRACKL_SONGTITLE, new QTableWidgetItem(song->getSongTitle(), key));
        ui_->twTrackList->setItem(row, TRACKL_SONGINTERPRET, new QTableWidgetItem(song->getSongInterpret(), key));
        ui_->twTrackList->setItem(row, TRACKL_SONGDURATION, new QTableWidgetItem(song->getSongDurationAsMinuteSecond(), key));
        ++row;
    }
}

void ArkEchoPlayerView::setLblDuration()
{
    if (!player_) return;
    /*qint64 duration = player_->duration();
    if (duration == 0) return;
    QString durationOverall = MusicSong::convertSongDurationToMinuteSecond(duration - MEDIAPLAYER_BUFFER_DURATION);*/

    qint64 position = player_->position();
    if (position >= MEDIAPLAYER_BUFFER_DURATION) position -= MEDIAPLAYER_BUFFER_DURATION;
    QString durationNow = MusicSong::convertMillisecondToMinuteSecond(position);

    QString text = durationNow;// +"/" + durationOverall;
    ui_->lblDuration->setText(text);
}

void ArkEchoPlayerView::setActualSongInfo(bool defaultText)
{
    // Song Cover
    QImage image = MusicSong::getAlbumCoverArt(player_);
    if (image.bits() == 0)  image = QImage("./Resources/defaultMusicIcon.png");
    ui_->lblCoverArt->setPixmap(QPixmap::fromImage(image));

    // Song Titel
    QString songTitle = MusicSong::getSongTitle(player_);
    if(defaultText) songTitle = ACTUAL_SONG_INFO_TEXT_DEFAULT;
    else if (songTitle.isEmpty()) songTitle = ACTUAL_SONG_INFO_TEXT_EMPTY;
    ui_->lblSongTitle->setText(songTitle);

    // Song Interpret
    QString songInterpret = MusicSong::getSongInterpret(player_);
    if (defaultText) songInterpret = ACTUAL_SONG_INFO_TEXT_DEFAULT;
    else if (songInterpret.isEmpty()) songInterpret = ACTUAL_SONG_INFO_TEXT_DEFAULT;
    ui_->lblSongInterpret->setText(songInterpret);

    // Album Titel
    QString albumTitle = MusicSong::getAlbumTitle(player_);
    if (defaultText) albumTitle = ACTUAL_SONG_INFO_TEXT_DEFAULT;
    else if (albumTitle.isEmpty()) albumTitle = ACTUAL_SONG_INFO_TEXT_DEFAULT;
    ui_->lblAlbumTitle->setText(albumTitle);

    QString albumInterpret = MusicSong::getAlbumInterpret(player_);
    if (defaultText) albumInterpret = ACTUAL_SONG_INFO_TEXT_DEFAULT;
    else if (albumInterpret.isEmpty()) albumInterpret = ACTUAL_SONG_INFO_TEXT_DEFAULT;
    ui_->lblAlbumInterpret->setText(albumInterpret);

    if (!model_ || defaultText) return;
    model_->sendActualSongInfo(image, songTitle, songInterpret, albumTitle, albumInterpret);
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
        onPbBackwardClicked();
        break;
    case REMOTE_BUTTON_FORWARD:
        onPbForwardClicked();
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
    if (!model_) return;
    model_->backwardPlaylist();
}

void ArkEchoPlayerView::onPbForwardClicked()
{
    if (!model_) return;
    model_->forwardPlaylist();
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
    int selectedKey = item->type();

    if (!model_) return;
    QList<int> keys;
    int rowCount = ui_->twTrackList->rowCount();
    for (int i = 0; i < rowCount; ++i)
    {
        keys.append(ui_->twTrackList->item(i, 0)->type());
    }
    model_->setMediaPlaylist(keys, selectedKey);

    if (!player_) return;
    player_->play();
    setLblDuration();
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
    setLblDuration();
    double duration = (double)player_->duration();
    double positionD = (double)position;

    if (duration == 0) return;
    double value = (positionD / duration) * 100;
    ui_->sliderDuration->setValue(value);
}

void ArkEchoPlayerView::onSliderDurationPressed()
{
    if (!player_) return;
    player_->pause();
}

void ArkEchoPlayerView::onSliderDurationReleased()
{
    if (!player_) return;
    double duration = (double)player_->duration();
    double value = ui_->sliderDuration->value();
    double newPosition = (duration / 100) * value;
    player_->setPosition(newPosition);
    player_->play();
    setLblDuration();
}

void ArkEchoPlayerView::onLeFilterTextChanged(const QString & text)
{
    setTWTrackList(text);
}

void ArkEchoPlayerView::onPbClearFilterClicked()
{
    ui_->leFilter->clear();
}

void ArkEchoPlayerView::onPbShuffleClicked()
{
    if (!model_) return;
    model_->shufflePlaylist();
}

void ArkEchoPlayerView::onPlayerMediaStatusChanged(const QMediaPlayer::MediaStatus & status)
{
    if (status == QMediaPlayer::MediaStatus::BufferedMedia || status == QMediaPlayer::MediaStatus::LoadedMedia)
    {
        setActualSongInfo();
    }
}
