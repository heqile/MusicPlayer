#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QtWidgets>
#include <QMultimedia>
#include <QMediaPlayer>
#include <QMediaPlaylist>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QPoint dragPos;
    QList<QString> list;
    QMediaPlayer *player;
    QMediaPlaylist *playlist;

    void setContextMenu();
    void setListMenu(QTableWidget* list);

    QAction *actionExit;
    QAction *actionAddSong;
    QAction *actionPlay;
    QAction *actionPause;
    QAction *actionPrev;
    QAction *actionNext;
    QAction *actionStop;
    QAction *actionHideorShowList;

    QAction *actionSeperate_1;
    QAction *actionSeperate_2;
    QAction *actionSeperate_3;

protected:
void mousePressEvent(QMouseEvent *);
void mouseMoveEvent(QMouseEvent *);

private slots:
    void slotExit();
    void slotPlayorPause();
    void slotPlay();
    void slotPause();
    void slotPrev();
    void slotNext();
    void slotStop();
    void slotAddSong();
    void slotPositionChange(qint64 position);
    void slotDurationChange(qint64 position);
    void slotSetPosition(int position);
    void slotHideorShowList();

};

#endif // MAINWINDOW_H
