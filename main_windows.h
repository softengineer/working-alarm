#ifndef MAIN_WINDOWS_H
#define MAIN_WINDOWS_H

#include <QDialog>
#include <QSystemTrayIcon>
#include <QObject>
#include <QWidget>
#include <QDebug>
#include <QCloseEvent>
#include <setting.h>

class QGridLayout;
class QLabel;
class QProgressBar;
class QPushButton;
class QImage;
class QTimer;
class QComboBox;

const QString WORKTIP = "Seated Working time left : ";
const QString STANDINGTIP = "Standing Working time left : ";
const QString RESTTIP = "Resting time left : ";

const int ST_SEATWORK = 1;
const int ST_STANDING = 2;
const int ST_REST = 3;



class Main_windows : public QDialog
{
    Q_OBJECT
public:
    Main_windows();

public slots:

     void reloadConfig();

private slots:


    void about();
    void start();
    void resume();
    void stop();
    void acknowledge();
    void showSetting();
    void timeout();


private:
    QAtomicInt totalSeatedSecondTime = 0;
    QAtomicInt totalStandSecondTime = 0;
    QAtomicInt totalRestSecondTime = 0;
    //0 stop
    //1 running
    //2 pause
    QAtomicInt runningStatus = 0;
    void init_window();
    void createTrayIcon();
    void createActions();
    void setAnimation();
    int  currentStauts();

    QString formatTime(int time);

    void closeEvent(QCloseEvent *closeEvent);


    QGridLayout * mainLayout;
    QProgressBar * pbar;
    QImage *   qiAlarm;
    QLabel * icon;
    QPushButton * btAbout;
    QPushButton * btAcknowledge;
    QPushButton * btSetting;
    QPushButton * btStart;
    QPushButton * btMin;
    QPushButton * btQuit;
    QPushButton * btStop;
    QPushButton * btPause;
    QComboBox *   qcStatus;

    QLabel * message;
    QLabel * status;
    QLabel * timeLabel;
    QLabel * movieLabel;
    QLabel * stat;

    int seatedMinutes = 0;
    int standMinutes = 0;
    int restMinutes = 0;

    QCheckBox * workMode;

    QAction *minimizeAction;
    QAction *maximizeAction;
    QAction *restoreAction;
    QAction *quitAction;

    QTimer * timer;
    Setting * setting;

    QMovie * workGif;
    QMovie * idleGif;
    QMovie * readyGif;
    QMovie * clickGif;
    QMovie *  standWorkingGif;

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;


};

#endif // MAIN_WINDOWS_H
