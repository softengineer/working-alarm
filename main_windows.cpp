#include "main_windows.h"
#include <QApplication>
#include <QProgressBar>
#include <QIcon>
#include <QPushButton>
#include <QLayout>
#include <QLabel>
#include <QPixmap>
#include <QSize>
#include <QTimer>
#include <QComboBox>
#include <QMenu>
#include <QMessageBox>



Main_windows::Main_windows()
{
    createActions();
    createTrayIcon();
    init_window();
}

void Main_windows::closeEvent(QCloseEvent *)
{
    // Do something
    QApplication::quit();
}

void Main_windows::init_window()
{
    Qt::WindowFlags flags = Qt::Window | Qt::WindowSystemMenuHint
                                | Qt::WindowMinimizeButtonHint
                                | Qt::WindowCloseButtonHint;

    setting = NULL;
    setWindowTitle(tr("Workout Alarm"));
    setWindowIcon(QIcon("images/trash.png"));
    setWindowFlags(flags);

    //control progress bar


    // control work sleep
    timer = new QTimer(this);
    // set time trigger every 1s
    timer->setInterval(1000);

    mainLayout = new QGridLayout;
    icon = new QLabel(tr("aa"));
    pbar = new QProgressBar(this);
    pbar->setFixedWidth(400);
    btAbout = new QPushButton(tr("About"));
    qiAlarm = new QImage( "images/alarm.png");
    icon->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
    icon->setPixmap(QPixmap::fromImage(*qiAlarm).scaled(25,25,Qt::KeepAspectRatio));

    btAcknowledge = new QPushButton(tr("Acknowledge"));
    btAcknowledge->setEnabled(false);

    btMin = new QPushButton(tr("Go tray"));
    btQuit = new QPushButton(tr("Quit"));

    pbar->setRange(0, 360);
    pbar->setValue(0);

    btStart = new QPushButton(tr("Start"));
    btSetting = new QPushButton(tr("Setting"));

    timeLabel = new QLabel(WORKTIP + " 00 h 00 m 00 s");
    status =    new QLabel(tr("Initial status : "));
    message = new QLabel(tr("Please click Start button to continue ..."));

    qcStatus = new QComboBox();
    qcStatus->addItem(tr("Working"));
    qcStatus->addItem(tr("Resting"));


    mainLayout->addWidget(icon, 0, 1, 1, 1,  Qt::AlignLeft);
    mainLayout->addWidget(pbar, 0, 2, 1, 6);
    mainLayout->addWidget(btAbout, 0, 8, 1, 1, Qt::AlignLeft);

    mainLayout->addWidget(message, 1, 1, 1, 10);
    mainLayout->addWidget(status, 2, 1, 1, 2);
    mainLayout->addWidget(qcStatus, 2, 3, 1, 2);
    mainLayout->addWidget(timeLabel, 3, 1, 1, 2);

    mainLayout->addWidget(btStart, 4, 2, 1, 2);
    mainLayout->addWidget(btAcknowledge, 4, 4, 1, 2);
    mainLayout->addWidget(btMin, 4, 6, 1,2);
    mainLayout->addWidget(btSetting, 4, 8, 1,2);
    mainLayout->addWidget(btQuit, 4, 10, 1,2);


    mainLayout->setHorizontalSpacing(2);
    mainLayout->setVerticalSpacing(10);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    setLayout(mainLayout);
    mainLayout->setSizeConstraint(QLayout::SetMinimumSize);

    // slots
    connect(btQuit, &QAbstractButton::clicked, this, &QApplication::quit);
    connect(btSetting, &QAbstractButton::clicked, this, &Main_windows::showSetting);
    connect(btStart, &QAbstractButton::clicked, this, &Main_windows::start);
    connect(btAcknowledge, &QAbstractButton::clicked, this, &Main_windows::acknowledge);
    connect(btMin, &QAbstractButton::clicked, this, &Main_windows::hide);
    connect(btAbout, &QAbstractButton::clicked, this, &Main_windows::about);
    // timer events
    connect(timer, SIGNAL(timeout()), this, SLOT(timeout()));


    resize(400, 300);
}

void Main_windows::showSetting()
{
  if (!setting)
      setting = new Setting();
   setting->show();
}

void Main_windows::timeout()
{
    int value = pbar->value() + 1;
    pbar->setValue(value);

    if (pbar->maximum()- value == 0) {
        if (currentStauts()==ST_WORK) {
            qcStatus->setCurrentText("Resting");
        } else {
            qcStatus->setCurrentText("Working");
        }
    }

    if (currentStauts() == ST_WORK) {
        timeLabel->setText( WORKTIP + formatTime(pbar->maximum()- value));
        //trayIcon->setToolTip(WORKTIP + formatTime(pbar->maximum()- value));
        if (pbar->maximum()- value == 0) {
             timer->stop();
             this->showNormal();
             this->activateWindow();
             btAcknowledge->setEnabled(true);
             message->setText("It's time to begin to work! Please click acknowledge button to continue...");
             QMessageBox::information(this, tr("Systray"),
                                      tr("It's time to begin to work! Please click acknowledge button to continue..."));
        }
     } else {
        timeLabel->setText( RESTTIP + formatTime(pbar->maximum()- value));
        //trayIcon->setToolTip(RESTTIP + formatTime(pbar->maximum()- value));
        if (pbar->maximum()- value == 0) {
             timer->stop();
             this->showNormal();
             this->activateWindow();
             btAcknowledge->setEnabled(true);
             message->setText("It's time to standup and have a rest! Please click acknowledge button to continue...");
             QMessageBox::information(this, tr("Systray"),
                                      tr("It's time to standup and have a rest! Please click acknowledge button to continue..."));
        }
     }
}

QString Main_windows::formatTime(int val)
{
    int hours = val / (60 * 60);
    int minutes = (val - hours * 60 * 60) / 60;
    int seconds = val - hours * 60 * 60 - minutes * 60;
    return QString::number(hours).rightJustified(2, '0') + " H " + QString::number(minutes).rightJustified(2, '0') + " m " + QString::number(seconds).rightJustified(2, '0') + " s ";
}

void Main_windows::createActions()
{
    minimizeAction = new QAction(tr("Mi&nimize"), this);
    connect(minimizeAction, &QAction::triggered, this, &QWidget::hide);

    maximizeAction = new QAction(tr("Ma&ximize"), this);
    connect(maximizeAction, &QAction::triggered, this, &QWidget::showMaximized);

    restoreAction = new QAction(tr("&Restore"), this);
    connect(restoreAction, &QAction::triggered, this, &QWidget::showNormal);

    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);
}


void Main_windows::createTrayIcon()
{
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(minimizeAction);
    trayIconMenu->addAction(maximizeAction);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
    QIcon icon("images/trash.png");
    trayIcon->setIcon(icon);
    trayIcon->setVisible(true);
}

void Main_windows::about()
{
   QMessageBox::about(this, "Rest Nortify V 0.1", "Author : David Fan, Email : minhao.fan@gmail.com");
}

int Main_windows::currentStauts()
{
    QString val = qcStatus->currentText();
    if (val.toLower().startsWith("work")) {
        return ST_WORK;
    } else {
        return ST_REST;
    }
}


void Main_windows::start()
{
    pbar->setValue(0);

    if (currentStauts() == ST_WORK) {
        timer->start();
        // working 45 minutes
        pbar->setRange(0, 20 * 60);
        message->setText("You are in working status, please keep working ...");
        message->setStyleSheet("QLabel {  color : red; }");
    } else {
        timer->start();
        // having rest 10 minutes
        pbar->setRange(0, 10 * 60);
        message->setText("You are in rest status, please stand up and breathe some fresh air ...");
        message->setStyleSheet("QLabel { color : green; }");
    }
    qcStatus->setEnabled(false);
    btStart->setEnabled(false);
    btAcknowledge->setEnabled(false);
    hide();
}


void Main_windows::acknowledge()
{
    start();
}
