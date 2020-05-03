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

const QString WORKTIP = "Working time left : ";
const QString RESTTIP = "Resting time left : ";

const int ST_WORK = 1;
const int ST_REST = 2;

class Main_windows : public QDialog
{
    Q_OBJECT
public:
    Main_windows();

private slots:
    void about();
    void start();
    void acknowledge();
    void showSetting();
    void timeout();

private:
    void init_window();
    void createTrayIcon();
    void createActions();
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
    QComboBox *   qcStatus;

    QLabel * message;
    QLabel * status;
    QLabel * timeLabel;

    QAction *minimizeAction;
    QAction *maximizeAction;
    QAction *restoreAction;
    QAction *quitAction;

    QTimer * timer;
    Setting * setting;

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
};

#endif // MAIN_WINDOWS_H
