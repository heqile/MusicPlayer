#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);

    ui->tableWidget->insertColumn(0);
    ui->tableWidget->insertColumn(1);
    ui->tableWidget->insertColumn(2);
    ui->tableWidget->hideColumn(2);
    ui->tableWidget->hideColumn(1);

    ui->tableWidget->setColumnWidth(0,175);
    ui->tableWidget->setColumnWidth(1,50);
    ui->tableWidget->horizontalHeader()->setMaximumSectionSize(175);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<"name"<<"form");
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setContextMenuPolicy(Qt::ActionsContextMenu);

   //ui->tableWidget->setAcceptDrops(true);
   // setAcceptDrops(true);
    //ui->tableWidget->
    ui->toolButton_Play->setText("Play");
    ui->toolButton_Play->setAutoRaise(true);
    ui->toolButton_Next->setAutoRaise(true);
    ui->toolButton_Prev->setAutoRaise(true);
    ui->toolButton_Stop->setAutoRaise(true);

    actionExit = new QAction(this);
    actionExit->setText(tr("&Exit"));
    actionAddSong = new QAction(this);
    actionAddSong->setText(tr("&Add_File"));
    actionPlay = new QAction(this);
    actionPlay->setText(tr("&Play"));
    actionPause = new QAction(this);
    actionPause->setText("&Pause");
    actionPrev = new QAction(this);
    actionPrev->setText("&Previous");
    actionNext = new QAction(this);
    actionNext->setText("&Next");
    actionStop = new QAction(this);
    actionStop->setText("&Stop");
    actionHideorShowList = new QAction(this);
    actionHideorShowList->setText("&Hide_List");

    setContextMenu();

    player = new QMediaPlayer(this);
    playlist = new QMediaPlaylist(this);
    playlist->setPlaybackMode(QMediaPlaylist::Loop);

    player->setPlaylist(playlist);
 //   setListMenu(ui->tableWidget);


    connect(actionExit,SIGNAL(triggered(bool)),this,SLOT(slotExit()));
    connect(actionAddSong,SIGNAL(triggered(bool)),this,SLOT(slotAddSong()));
    connect(actionPlay,SIGNAL(triggered(bool)),this,SLOT(slotPlay()));
    connect(actionPause,SIGNAL(triggered(bool)),this,SLOT(slotPause()));
    connect(actionPrev,SIGNAL(triggered(bool)),this,SLOT(slotPrev()));
    connect(actionNext,SIGNAL(triggered(bool)),this,SLOT(slotNext()));
    connect(actionStop,SIGNAL(triggered(bool)),this,SLOT(slotStop()));
    connect(actionHideorShowList,SIGNAL(triggered(bool)),this,SLOT(slotHideorShowList()));

    connect(ui->toolButton_Play,SIGNAL(clicked(bool)),this,SLOT(slotPlayorPause()));
    connect(ui->toolButton_Stop,SIGNAL(clicked(bool)),this,SLOT(slotStop()));
    connect(ui->toolButton_Next,SIGNAL(clicked(bool)),this,SLOT(slotNext()));
    connect(ui->toolButton_Prev,SIGNAL(clicked(bool)),this,SLOT(slotPrev()));

    connect(player,SIGNAL(positionChanged(qint64)),this,SLOT(slotPositionChange(qint64)));
    connect(player,SIGNAL(durationChanged(qint64)),this,SLOT(slotDurationChange(qint64)));
    connect(ui->sliderProgress,SIGNAL(sliderMoved(int)),this,SLOT(slotSetPosition(int)));

//    connect(ui->toolButton_Play,SIGNAL(clicked(bool)),this,SLOT()

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setContextMenu()
{
    actionSeperate_1 = new QAction(this);
    actionSeperate_1->setSeparator(true);
    actionSeperate_2 = new QAction(this);
    actionSeperate_2->setSeparator(true);
    actionSeperate_3 = new QAction(this);
    actionSeperate_3->setSeparator(true);

    this->addAction(actionAddSong);
    this->addAction(actionHideorShowList);
    this->addAction(actionSeperate_1);
    this->addAction(actionPlay);
    this->addAction(actionPause);
    this->addAction(actionStop);
    this->addAction(actionSeperate_2);
    this->addAction(actionPrev);
    this->addAction(actionNext);
    this->addAction(actionSeperate_3);
    this->addAction(actionExit);
    this->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void MainWindow::setListMenu(QTableWidget* list)
{
    list->addAction(actionPlay);
}

void MainWindow::slotExit()
{
    this->close();
}

void MainWindow::slotPlayorPause()
{
    if(ui->toolButton_Play->text() == "Play")
        slotPlay();
    else
        slotPause();
}

void MainWindow::slotPlay()
{
    ui->toolButton_Play->setText("Pause");
    ui->toolButton_Play->setIcon(QIcon(":/images/Pause"));
    if(!(playlist->currentIndex() == ui->tableWidget->currentRow()))
    {
        playlist->setCurrentIndex(ui->tableWidget->currentRow());
        player->setMedia(playlist->currentMedia());
    }
    player->play();
}

void MainWindow::slotPause()
{
    ui->toolButton_Play->setText("Play");
    ui->toolButton_Play->setIcon(QIcon(":/images/Play"));
    player->pause();
}

void MainWindow::slotPrev()
{
    playlist->previous();
    slotPlay();
}

void MainWindow::slotNext()
{
    playlist->next();
}

void MainWindow::slotStop()
{
    ui->toolButton_Play->setText("Play");
    ui->toolButton_Play->setIcon(QIcon(":/images/Play"));
    player->stop();
}

void MainWindow::slotAddSong()
{
    QStringList filepaths = QFileDialog::getOpenFileNames(this,"open a file","/home/lele/MusicPlayer","music_file(*.mp3)");
    QString filepath;
    foreach(filepath, filepaths)
    {
        if(!filepath.isEmpty())
        {
            if(!list.contains(filepath))
            {
                list.append(filepath);
                playlist->addMedia(QUrl::fromLocalFile(filepath));
                QString filename = filepath.split("/").last();
                int rownumber = ui->tableWidget->rowCount();
                ui->tableWidget->insertRow(rownumber);
                ui->tableWidget->setItem(rownumber,0,new QTableWidgetItem(filename));
//                ui->tableWidget->setItem(rownumber,1,new QTableWidgetItem(filename.split(".").last()));
                ui->tableWidget->setItem(rownumber,2,new QTableWidgetItem(filepath));
            }
        }

    }
}

void MainWindow::slotPositionChange(qint64 position)
{
    ui->sliderProgress->setValue(position);
}

void MainWindow::slotDurationChange(qint64 position)
{
    ui->sliderProgress->setMaximum(position);
}

void MainWindow::slotSetPosition(int position)
{
    player->setPosition(position);
}

void MainWindow::slotHideorShowList()
{
    if(ui->tableWidget->isVisible())
    {
        ui->tableWidget->setVisible(false);
        ui->tableWidget->setDisabled(true);
        actionHideorShowList->setText("&Show_List");
    }
    else
    {
        ui->tableWidget->setVisible(true);
        ui->tableWidget->setDisabled(false);
        actionHideorShowList->setText("&Hide_List");
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {
        dragPos=event->globalPos() - frameGeometry().topLeft();
//event->accept();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() == Qt::LeftButton)
    {
        move(event->globalPos()-dragPos);
        //event->accept();
    }
}

