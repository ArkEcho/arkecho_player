#include "ArkEchoPlayerView.h"
#include "ArkEchoPlayerModel.h"
#include "MusicSongList.h"

#include <QLabel>
#include <QMediaPlayer>

const QString DIALOGTITLE = "ArkEcho Media Player";
// MetaData und Player Song Länge unterscheiden sich etwa um 560ms
const int MEDIAPLAYER_DURATION_BUFFER = 560;
const int DEFAULT_VOLUME = 100;
const int ACTUAL_SONG_INFO_TEXT_WIDTH = 150;
const int FILTER_LINEEDIT_WIDTH = 250;
const int VOLUME_TEXT_WIDTH = 20;
const QSize ACTUAL_SONG_INFO_COVER_SIZE = QSize(150, 150);
const QString ACTUAL_SONG_INFO_TEXT_DEFAULT = "<Kein Lied gestartet>";
const QString ACTUAL_SONG_INFO_TEXT_EMPTY = "<Keine Meta-Information>";
const QString BUTTON_ICON_RESOURCE_PATH = "./Resources/player/";
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
    connect(model_, SIGNAL(actualSongInfoChanged(SongInfoStruct)), this, SLOT(onActualSongInfoChanged(SongInfoStruct)));

    player_ = new QMediaPlayer();
    player_->setPlaylist(model_->getMediaPlaylist());
    connect(player_, SIGNAL(positionChanged(qint64)), this, SLOT(onPlayerPositionChanged(qint64)));

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
    ui_->lblVolume->setMinimumWidth(VOLUME_TEXT_WIDTH);
    ui_->lblVolume->setMaximumWidth(VOLUME_TEXT_WIDTH);
    ui_->lblVolume->setText(QString::number(DEFAULT_VOLUME));
    ui_->sliderVolume->setValue(DEFAULT_VOLUME);

    // Filter Line Edit initialisieren
    ui_->leFilter->setMinimumWidth(FILTER_LINEEDIT_WIDTH);
    ui_->leFilter->setMaximumWidth(FILTER_LINEEDIT_WIDTH);

    // PushButton Icons
    QSize buttonSize = QSize(40, 20);
    ui_->pbPlay_Pause->setText("");
    ui_->pbPlay_Pause->setIcon(QPixmap::fromImage(QImage(BUTTON_ICON_RESOURCE_PATH + "playPauseIcon.png")));
    ui_->pbPlay_Pause->setIconSize(QSize(60, 30));

    ui_->pbFilterClear->setText("");
    ui_->pbFilterClear->setIcon(QPixmap::fromImage(QImage(BUTTON_ICON_RESOURCE_PATH + "clearIcon.png")));
    ui_->pbFilterClear->setIconSize(QSize(40,16));

    ui_->pbBackward->setText("");
    ui_->pbBackward->setIcon(QPixmap::fromImage(QImage(BUTTON_ICON_RESOURCE_PATH + "rewindIcon.png")));
    ui_->pbBackward->setIconSize(buttonSize);

    ui_->pbForward->setText("");
    ui_->pbForward->setIcon(QPixmap::fromImage(QImage(BUTTON_ICON_RESOURCE_PATH + "forwardIcon.png")));
    ui_->pbForward->setIconSize(buttonSize);

    ui_->pbShuffle->setText("");
    ui_->pbShuffle->setIcon(QPixmap::fromImage(QImage(BUTTON_ICON_RESOURCE_PATH + "shuffleIcon.png")));
    ui_->pbShuffle->setIconSize(buttonSize);

    ui_->pbStop->setText("");
    ui_->pbStop->setIcon(QPixmap::fromImage(QImage(BUTTON_ICON_RESOURCE_PATH + "stopIcon.png")));
    ui_->pbStop->setIconSize(buttonSize);

    // Actual Song Info initalisieren
    ui_->lblCoverArt->setScaledContents(true);
    ui_->lblCoverArt->setMinimumSize(ACTUAL_SONG_INFO_COVER_SIZE);
    ui_->lblCoverArt->setMaximumSize(ACTUAL_SONG_INFO_COVER_SIZE);
    ui_->lblSongTitle->setMaximumWidth(ACTUAL_SONG_INFO_TEXT_WIDTH);
    ui_->lblSongInterpret->setMaximumWidth(ACTUAL_SONG_INFO_TEXT_WIDTH);
    ui_->lblAlbumTitle->setMaximumWidth(ACTUAL_SONG_INFO_TEXT_WIDTH);
    ui_->lblAlbumInterpret->setMaximumWidth(ACTUAL_SONG_INFO_TEXT_WIDTH);
    SongInfoStruct song;
    song.coverArt_ = QImage("./Resources/defaultMusicIcon.png");
    song.songTitle_ = ACTUAL_SONG_INFO_TEXT_DEFAULT;
    song.songInterpret_ = ACTUAL_SONG_INFO_TEXT_DEFAULT;
    song.albumTitle_ = ACTUAL_SONG_INFO_TEXT_DEFAULT;
    song.albumInterpret_ = ACTUAL_SONG_INFO_TEXT_DEFAULT;
    setActualSongInfo(song);
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

        // Filter Music Songs
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
        ////////////////////

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
    if (position >= MEDIAPLAYER_DURATION_BUFFER) position -= MEDIAPLAYER_DURATION_BUFFER;
    QString durationNow = MusicSong::convertMillisecondToMinuteSecond(position);

    QString text = durationNow;// +"/" + durationOverall;
    ui_->lblDuration->setText(text);
}

void ArkEchoPlayerView::setActualSongInfo(SongInfoStruct& sis)
{
    // Song Cover
    ui_->lblCoverArt->setPixmap(QPixmap::fromImage(sis.coverArt_));

    // Song Titel
    if (sis.songTitle_.isEmpty()) sis.songTitle_ = ACTUAL_SONG_INFO_TEXT_EMPTY;
    ui_->lblSongTitle->setText(sis.songTitle_);

    // Song Interpret
    if (sis.songInterpret_.isEmpty()) sis.songInterpret_ = ACTUAL_SONG_INFO_TEXT_EMPTY;
    ui_->lblSongInterpret->setText(sis.songInterpret_);

    // Album Titel
    if (sis.albumTitle_.isEmpty()) sis.albumTitle_ = ACTUAL_SONG_INFO_TEXT_EMPTY;
    ui_->lblAlbumTitle->setText(sis.albumTitle_);

    // Album Interpret
    if (sis.albumInterpret_.isEmpty()) sis.albumInterpret_ = ACTUAL_SONG_INFO_TEXT_EMPTY;
    ui_->lblAlbumInterpret->setText(sis.albumInterpret_);
}

void ArkEchoPlayerView::onUpdateView(const int &uve)
{
    switch(uve)
    {
    case WEBSOCKET_CONNECTED:
        setWebSocketStatusLabel(true);
        break;
    case WEBSOCKET_DISCONNECTED:
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
    case REQUEST_SONG_ACTUAL_BY_SOCKET:
        // TODO: Einbauen
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

void ArkEchoPlayerView::onActualSongInfoChanged(const SongInfoStruct & sis)
{
    SongInfoStruct sisCopy = sis;
    setActualSongInfo(sisCopy);
}
