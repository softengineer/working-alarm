#include "main_windows.h"
#include "setting.h"
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
#include <QMovie>
#include <QCheckBox>



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

    setting = new Setting();

    QObject::connect(setting, SIGNAL(updateChange( )),      this,  SLOT(reloadConfig()));
    setWindowTitle(tr("Working/Rest Alarm"));
    setWindowIcon(QIcon(":images/alarm_app.ico"));
    setWindowFlags(flags);


    workGif = new QMovie(":images/working.gif");
    idleGif = new QMovie(":images/giphy.gif");
    readyGif = new QMovie(":images/ready.gif");
    clickGif = new QMovie(":images/click.gif");
    standWorkingGif =  new QMovie(":images/standing_working.gif");

    stat = new QLabel( QString("Statistic:\nSeated Minutes: %1m\nStanding Minutes: %1m\nRest Minutes: %1m").
                       arg(formatTime(totalSeatedSecondTime)).arg(formatTime(totalStandSecondTime)).arg(formatTime(totalRestSecondTime)) );

    // control work sleep
    timer = new QTimer(this);
    // set time trigger every 1s
    timer->setInterval(1000);

    acknowledgeTimer = new QTimer(this);
    // set time trigger every 1s
    acknowledgeTimer->setInterval(1000);

    mainLayout = new QGridLayout;
    icon = new QLabel(tr("aa"));
    pbar = new QProgressBar(this);
    pbar->setFixedWidth(400);
    btAbout = new QPushButton(tr("About"));
    qiAlarm = new QImage( ":images/alarm.png");
    icon->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
    icon->setPixmap(QPixmap::fromImage(*qiAlarm).scaled(25,25,Qt::KeepAspectRatio));

    btAcknowledge = new QPushButton(tr("Acknowledge"));
    btAcknowledge->setEnabled(false);

    btMin = new QPushButton(tr("Go tray"));
    btQuit = new QPushButton(tr("Quit"));

    pbar->setRange(0, 360);
    pbar->setValue(0);

    btStart = new QPushButton(tr("Start"));
    btPause = new QPushButton(tr("Pause"));
    btStop = new QPushButton(tr("Stop"));
    btSetting = new QPushButton(tr("Setting"));

    movieLabel = new QLabel();
    movieLabel->setGeometry(115,60,128,128);
    timeLabel = new QLabel(WORKTIP + " 00 h 00 m 00 s");
    status =    new QLabel(tr("Status : "));
    workMode = new QCheckBox(tr("Work Cycle Mode "));
    workMode->setChecked(true);

    autoAcknowledge =  new QCheckBox(tr("Auto Acknowledge"));
    autoAcknowledge->setChecked(false);
    message = new QLabel(tr("Please click Start button to continue ..."));

    qcStatus = new QComboBox();
    qcStatus->addItem(tr("Seated Working"));
    qcStatus->addItem(tr("Standing Working"));
    qcStatus->addItem(tr("Resting"));


    mainLayout->addWidget(icon, 0, 1, 1, 1,  Qt::AlignLeft);
    mainLayout->addWidget(pbar, 0, 2, 1, 6);
    mainLayout->addWidget(btAbout, 0, 7, 1, 1, Qt::AlignLeft);

    mainLayout->addWidget(message, 1, 1, 1, 10);
    mainLayout->addWidget(status, 2, 1, 1, 2);
    mainLayout->addWidget(qcStatus, 2, 3, 1, 2);
    mainLayout->addWidget(workMode, 3, 1, 1, 2);
    mainLayout->addWidget(stat, 3, 3, 1, 2);
    mainLayout->addWidget(autoAcknowledge, 4, 1, 1, 2);
    mainLayout->addWidget(timeLabel, 4, 3, 1, 2);

    mainLayout->addWidget(movieLabel, 1, 9, 5, 2, Qt::AlignRight);

    mainLayout->addWidget(btStart, 5, 0, 1, 2);
    mainLayout->addWidget(btPause, 5, 2, 1, 2);
    mainLayout->addWidget(btStop, 5, 4, 1, 2);
    mainLayout->addWidget(btAcknowledge, 5, 6, 1, 2);
    mainLayout->addWidget(btMin, 5, 8, 1,2);
    mainLayout->addWidget(btSetting, 5, 10, 1,2);
    mainLayout->addWidget(btQuit, 5, 12, 1,2);


    mainLayout->setHorizontalSpacing(2);
    mainLayout->setVerticalSpacing(10);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    setLayout(mainLayout);
    mainLayout->setSizeConstraint(QLayout::SetMinimumSize);

    movieLabel->setMovie(readyGif);
    readyGif->start();

    // slots
    connect(btQuit, &QAbstractButton::clicked, this, &QApplication::quit);
    connect(btSetting, &QAbstractButton::clicked, this, &Main_windows::showSetting);
    connect(btStart, &QAbstractButton::clicked, this, &Main_windows::start);
    connect(btPause, &QAbstractButton::clicked, this, &Main_windows::resume);
    connect(btStop, &QAbstractButton::clicked, this, &Main_windows::stop);
    connect(btAcknowledge, &QAbstractButton::clicked, this, &Main_windows::acknowledge);
    connect(btMin, &QAbstractButton::clicked, this, &Main_windows::hide);
    connect(btAbout, &QAbstractButton::clicked, this, &Main_windows::about);
    // timer events
    connect(timer, SIGNAL(timeout()), this, SLOT(timeout()));

    // timer events
    connect(acknowledgeTimer, SIGNAL(timeout()), this, SLOT(acknowledgeTimeout()));
    resize(400, 300);
}

void Main_windows::showSetting()
{
   setting->show();
}

void Main_windows::acknowledgeTimeout() {
    acknowledgeTime--;
    if (acknowledgeTime == 0) {

        btAcknowledge->click();
    }
    btAcknowledge->setText(tr("Acknowledge") + tr("(")  + QString::number(acknowledgeTime) + tr(")"));

}

void Main_windows::timeout()
{
    if (runningStatus == 2) {
        return;
    }

    int value = pbar->value() + 1;
    pbar->setValue(value);

    if (currentStauts() == ST_SEATWORK) {
        totalSeatedSecondTime += 1;
    } else if (currentStauts() == ST_STANDING) {
        totalStandSecondTime += 1;
    } else {
        totalRestSecondTime += 1;
     }


    if (pbar->maximum()- value == 0) {
        btStop->setEnabled(true);
        if (currentStauts()== ST_SEATWORK) {
            workGif->stop();
            qcStatus->setCurrentText("Standing Working");
        } else if (currentStauts()== ST_STANDING) {
            standWorkingGif->stop();
            if (workMode->isChecked()) {
                qcStatus->setCurrentText("Seated Working");
            } else {
                qcStatus->setCurrentText("Resting");
            }
        }
        else {
            idleGif->stop();
            qcStatus->setCurrentText("Seated Working");
        }

        movieLabel->setMovie(clickGif);
        clickGif->start();
    }

    if (currentStauts() == ST_SEATWORK) {

        timeLabel->setText( WORKTIP + formatTime(pbar->maximum()- value));

        if (pbar->maximum()- value == 0) {
             timer->stop();
             this->showNormal();
             this->activateWindow();
             btAcknowledge->setEnabled(true);
              btAcknowledge->setStyleSheet("background-color:red;");
              if (this->autoAcknowledge->isChecked()) {
                  acknowledgeTime = totalAcknowledgeTime;
                     btAcknowledge->setText(tr("Acknowledge") + tr("(")  + QString::number(acknowledgeTime) + tr(")"));

                     acknowledgeTimer->start();

              }
             message->setText("It's time to begin to seated work! Please click acknowledge button to continue...");
             QMessageBox::information(this, tr("Systray"),
                                      tr("It's time to begin to do seated work! Please click acknowledge button to continue..."));

        }
     } else if (currentStauts() == ST_STANDING) {

        timeLabel->setText( STANDINGTIP + formatTime(pbar->maximum()- value));

        if (pbar->maximum()- value == 0) {
             timer->stop();
             this->showNormal();
             this->activateWindow();
             btAcknowledge->setEnabled(true);
             btAcknowledge->setStyleSheet("background-color:red;");
             if (this->autoAcknowledge->isChecked()) {
                    acknowledgeTime = totalAcknowledgeTime;
                    btAcknowledge->setText(tr("Acknowledge") + tr("(")  + QString::number(acknowledgeTime) + tr(")"));

                    acknowledgeTimer->start();

             }
             message->setText("It's time to begin to standing work! Please click acknowledge button to continue...");
             QMessageBox::information(this, tr("Systray"),
                                      tr("It's time to begin to do standing work! Please click acknowledge button to continue..."));

        }
     } else {
        timeLabel->setText( RESTTIP + formatTime(pbar->maximum()- value));


        if (pbar->maximum()- value == 0) {
             timer->stop();
             this->showNormal();
             this->activateWindow();
             btAcknowledge->setEnabled(true);
             btAcknowledge->setStyleSheet("background-color:red;");
             if (this->autoAcknowledge->isChecked()) {
                    acknowledgeTime = totalAcknowledgeTime;
                    btAcknowledge->setText(tr("Acknowledge") + tr("(")  + QString::number(acknowledgeTime) + tr(")"));

                    acknowledgeTimer->start();

             }
             message->setText("It's time to standup and have a rest! Please click acknowledge button to continue...");
             QMessageBox::information(this, tr("Systray"),
                                      tr("It's time to have a rest! Please click acknowledge button to continue..."));
        }
     }

    if (currentStauts() == ST_SEATWORK) {
        QString text = "Statistic:<br><b>Seated Minutes: " +formatTime(totalSeatedSecondTime) + "</b><br>Standing Minutes: "
                + formatTime(totalStandSecondTime)+"<br>Rest Minutes: "+ formatTime(totalRestSecondTime);
        stat->setText(text);

    } else if (currentStauts() == ST_STANDING) {
        QString text = "Statistic:<br>Seated Minutes: " +formatTime(totalSeatedSecondTime) + "<br><b>Standing Minutes: "
                + formatTime(totalStandSecondTime)+"</b><br>Rest Minutes: "+ formatTime(totalRestSecondTime);
        stat->setText(text);

    } else {
        QString text = "Statistic:<br>Seated Minutes: " +formatTime(totalSeatedSecondTime) + "<br>Standing Minutes: "
                + formatTime(totalStandSecondTime)+"<br><b>Rest Minutes: "+ formatTime(totalRestSecondTime) + "</b>";
        stat->setText(text);
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
    QIcon icon("images/alarm_app.ico");
    trayIcon->setIcon(icon);
    trayIcon->setVisible(true);
}

void Main_windows::about()
{
   QMessageBox::about(this, "Rest Nortify V 0.1", "Author : David Fan\nEmail : minhao.fan@gmail.com\nHomepage: https://github.com/softengineer");
}

int Main_windows::currentStauts()
{
    QString val = qcStatus->currentText();
    if (val.toLower().startsWith("seat")) {
        return ST_SEATWORK;
    } else if (val.toLower().startsWith("stand")) {
        return ST_STANDING;
    }  else {
        return ST_REST;
    }
}

void Main_windows::setAnimation()
{
    clickGif->stop();
    readyGif->stop();

    if (currentStauts() == ST_SEATWORK) {
         movieLabel->setMovie(workGif);
         workGif->start();
    } else if (currentStauts() == ST_STANDING) {
        movieLabel->setMovie(standWorkingGif);
        standWorkingGif->start();
   } else {
         movieLabel->setMovie(idleGif);
         idleGif->start();
    }
}

void Main_windows::reloadConfig() {
    if (currentStauts() == ST_SEATWORK) {
        timer->start();
        // working 45 minutes
        pbar->setRange(0,  setting->getTime("seatedwork") * 60);
        message->setText("You are in seated working status, please keep working ...");
        message->setStyleSheet("QLabel {  color : red; }");
        // only work we hide the windows
        //hide();
    } else  if (currentStauts() == ST_STANDING) {
        timer->start();
        // working 45 minutes
        pbar->setRange(0, setting->getTime("standwork") * 60);
        message->setText("You are in standing working status, please keep working ...");
        message->setStyleSheet("QLabel {  color : red; }");
        // only work we hide the windows
        //hide();
    } else {
        timer->start();
        // having rest 10 minutes
        pbar->setRange(0, setting->getTime("rest") * 60);
        message->setText("You are in rest status, please stand up and breathe some fresh air ...");
        message->setStyleSheet("QLabel { color : green; }");
    }
}


void Main_windows::start()
{
    runningStatus = 1;
    pbar->setValue(0);

    qcStatus->setEnabled(false);
    btStart->setEnabled(false);
    btStop->setEnabled(true);
    btAcknowledge->setEnabled(false);
    btAcknowledge->setStyleSheet("");
    setAnimation();
    reloadConfig();
}

void Main_windows::stop()
{
    runningStatus = 0;
    timer->stop();
    int value = pbar->value() + 1;

    pbar->setValue(0);

    qcStatus->setEnabled(true);
    btStop->setEnabled(false);
    btStart->setEnabled(true);
    btAcknowledge->setEnabled(false);

    pbar->setValue(value);

        if (currentStauts()== ST_SEATWORK) {


            workGif->stop();

        } else if (currentStauts()== ST_STANDING) {

            standWorkingGif->stop();

        }
        else {
            idleGif->stop();

        }

        movieLabel->setMovie(clickGif);
    }


void Main_windows::resume()
{
    if (runningStatus == 2) {
          runningStatus = 1;
          btPause->setText("Pause");
          setWindowTitle(tr("Working/Rest Alarm"));
    } else if  (runningStatus == 1)  {
         runningStatus = 2;
         btPause->setText("Resume");
         setWindowTitle(tr("Working/Rest Alarm(Pause ....)"));
    }
}



void Main_windows::acknowledge()
{
    acknowledgeTimer->stop();
    start();
}
