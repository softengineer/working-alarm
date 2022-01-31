/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "setting.h"

#ifndef QT_NO_SYSTEMTRAYICON

#include <QAction>
#include <QCheckBox>
#include <QComboBox>
#include <QCoreApplication>
#include <QCloseEvent>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QMenu>
#include <QPushButton>
#include <QSpinBox>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QJsonDocument>
#include <QFile>
#include <QJsonObject>
#include <QJsonArray>

//! [0]
Setting::Setting()
{

    this->loadJsonSetting();
//    createIconGroupBox();
//    createMessageGroupBox();
    createTimeSettingGroupBox();

//    iconLabel->setMinimumWidth(durationLabel->sizeHint().width());

    createActions();
//    createTrayIcon();

//    connect(showMessageButton, &QAbstractButton::clicked, this, &Setting::showMessage);
//    connect(showIconCheckBox, &QAbstractButton::toggled, trayIcon, &QSystemTrayIcon::setVisible);
//    connect(iconComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
//            this, &Setting::setIcon);
//    connect(trayIcon, &QSystemTrayIcon::messageClicked, this, &Setting::messageClicked);
//    connect(trayIcon, &QSystemTrayIcon::activated, this, &Setting::iconActivated);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    //mainLayout->addWidget(iconGroupBox);
    //mainLayout->addWidget(messageGroupBox);
    mainLayout->addWidget(timeSettingGroupBox);


    setLayout(mainLayout);

//    iconComboBox->setCurrentIndex(1);
//    trayIcon->show();

    setWindowTitle(tr("Setting"));
    resize(400, 150);
}
//! [0]

//! [1]
void Setting::setVisible(bool visible)
{
    minimizeAction->setEnabled(visible);
    maximizeAction->setEnabled(!isMaximized());
    restoreAction->setEnabled(isMaximized() || !visible);
    QDialog::setVisible(visible);
}
//! [1]

//! [2]
void Setting::closeEvent(QCloseEvent *event)
{


        this->setTime("rest", restTimeSpinBox->value());
        this->setTime("seatedwork", seatedWorkTimeSpinBox->value());
        this->setTime("standwork", standWorkTimeSpinBox->value());
        saveJsonSetting();
        hide();
        event->ignore();

}
//! [2]

//! [3]
void Setting::setIcon(int index)
{
    QIcon icon = iconComboBox->itemIcon(index);
    trayIcon->setIcon(icon);
    setWindowIcon(icon);

    trayIcon->setToolTip(iconComboBox->itemText(index));
}
//! [3]

//! [4]
void Setting::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
        iconComboBox->setCurrentIndex((iconComboBox->currentIndex() + 1) % iconComboBox->count());
        break;
    case QSystemTrayIcon::MiddleClick:
        showMessage();
        break;
    default:
        ;
    }
}
//! [4]

//! [5]
void Setting::showMessage()
{
    showIconCheckBox->setChecked(true);
    int selectedIcon = typeComboBox->itemData(typeComboBox->currentIndex()).toInt();
    QSystemTrayIcon::MessageIcon msgIcon = QSystemTrayIcon::MessageIcon(selectedIcon);

    if (selectedIcon == -1) { // custom icon
        QIcon icon(iconComboBox->itemIcon(iconComboBox->currentIndex()));
        trayIcon->showMessage(titleEdit->text(), bodyEdit->toPlainText(), icon,
                          durationSpinBox->value() * 1000);
    } else {
        trayIcon->showMessage(titleEdit->text(), bodyEdit->toPlainText(), msgIcon,
                          durationSpinBox->value() * 1000);
    }
}
//! [5]

//! [6]
void Setting::messageClicked()
{
    QMessageBox::information(nullptr, tr("Systray"),
                             tr("Sorry, I already gave what help I could.\n"
                                "Maybe you should try asking a human?"));
}
//! [6]

void Setting::createIconGroupBox()
{
    iconGroupBox = new QGroupBox(tr("Tray Icon"));

    iconLabel = new QLabel("Icon:");

    iconComboBox = new QComboBox;
    iconComboBox->addItem(QIcon(":/images/bad.png"), tr("Bad"));
    iconComboBox->addItem(QIcon(":/images/heart.png"), tr("Heart"));
    iconComboBox->addItem(QIcon(":/images/trash.png"), tr("Trash"));

    showIconCheckBox = new QCheckBox(tr("Show icon"));
    showIconCheckBox->setChecked(true);

    QHBoxLayout *iconLayout = new QHBoxLayout;
    iconLayout->addWidget(iconLabel);
    iconLayout->addWidget(iconComboBox);
    iconLayout->addStretch();
    iconLayout->addWidget(showIconCheckBox);
    iconGroupBox->setLayout(iconLayout);
}

ConfigItem & ConfigItem::operator = ( ConfigItem & item) {
    this->key = item.key;
    this->value = item.value;
    this->category = item.category;

    return *this;
}

void Setting::loadJsonSetting() {

        QFile inFile(CONFIG_PATH);
        inFile.open(QIODevice::ReadOnly|QIODevice::Text);
        QByteArray data = inFile.readAll();
        inFile.close();

        QJsonParseError errorPtr;
        QJsonDocument doc = QJsonDocument::fromJson(data, &errorPtr);
        if (doc.isNull()) {
            qDebug() << "Parse failed";
        }
        QJsonArray array = doc.array();

        int length = array.size();
        ConfigItem * all = new ConfigItem[length];
        for (int i = 0; i< length; i++)
        {
            const QJsonValue &v =  array[i];

            QJsonObject obj = v.toObject();
            ConfigItem item;

            item.key  =  obj.value("key").toString();
            item.value = obj.value("value").toInt();
            item.category =  obj.value("category").toString();
            all[i] = item;
        }

        this->ConfigItems =  all;
}

bool Setting::saveJsonSetting() {

    QFile file(CONFIG_PATH);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QJsonParseError JsonParseError;
    QJsonDocument JsonDocument = QJsonDocument::fromJson(file.readAll(), &JsonParseError);
    file.close();
    QJsonObject rootObject = JsonDocument.object();
    QJsonArray array;
    QJsonObject qrest;
    qrest.insert("key", "rest");
    qrest.insert("value", getTime("rest"));
    qrest.insert("category", "rest");
    array.push_back(qrest);

    QJsonObject qseat;
    qseat.insert("key", "seatedwork");
    qseat.insert("value", getTime("seatedwork"));
    qseat.insert("category", "work");
    array.push_back(qseat);


    QJsonObject qstand;
    qstand.insert("key", "standwork");
    qstand.insert("value", getTime("standwork"));
    qstand.insert("category", "work");
    array.push_back(qstand);


    JsonDocument.setArray(array);
    file.open(QFile::WriteOnly | QFile::Text | QFile::Truncate);
    file.write(JsonDocument.toJson());
    file.close();

    return true;
}

int Setting::getTime( QString name) {
    int length = 3;//sizeof( this->ConfigItems ) / sizeof( ConfigItem );

    for (int j = 0; j< length; j++) {


        if (this->ConfigItems [ j].key == name) {
            return this->ConfigItems [ j].value;
        }
    }

    return -1;
}

void Setting::setTime( QString name, int value) {
    int length = sizeof( ConfigItem ) / sizeof( ConfigItem );

    for (int j = 0; j< length; j++) {
        ConfigItem * p = this->ConfigItems + j;
        if (p->key == name) {
             p->value = value;
        }
    }

    return;
}





void Setting::createMessageGroupBox()
{
    messageGroupBox = new QGroupBox(tr("Balloon Message"));

    typeLabel = new QLabel(tr("Type:"));

    typeComboBox = new QComboBox;
    typeComboBox->addItem(tr("None"), QSystemTrayIcon::NoIcon);
    typeComboBox->addItem(style()->standardIcon(
            QStyle::SP_MessageBoxInformation), tr("Information"),
            QSystemTrayIcon::Information);
    typeComboBox->addItem(style()->standardIcon(
            QStyle::SP_MessageBoxWarning), tr("Warning"),
            QSystemTrayIcon::Warning);
    typeComboBox->addItem(style()->standardIcon(
            QStyle::SP_MessageBoxCritical), tr("Critical"),
            QSystemTrayIcon::Critical);
    typeComboBox->addItem(QIcon(), tr("Custom icon"),
            -1);
    typeComboBox->setCurrentIndex(1);

    durationLabel = new QLabel(tr("Duration:"));

    durationSpinBox = new QSpinBox;
    durationSpinBox->setRange(5, 60);
    durationSpinBox->setSuffix(" s");
    durationSpinBox->setValue(15);

    durationWarningLabel = new QLabel(tr("(some systems might ignore this "
                                         "hint)"));
    durationWarningLabel->setIndent(10);

    titleLabel = new QLabel(tr("Title:"));

    titleEdit = new QLineEdit(tr("Cannot connect to network"));

    bodyLabel = new QLabel(tr("Body:"));

    bodyEdit = new QTextEdit;
    bodyEdit->setPlainText(tr("Don't believe me. Honestly, I don't have a "
                              "clue.\nClick this balloon for details."));

    showMessageButton = new QPushButton(tr("Show Message"));
    showMessageButton->setDefault(true);

    QGridLayout *messageLayout = new QGridLayout;
    messageLayout->addWidget(typeLabel, 0, 0);
    messageLayout->addWidget(typeComboBox, 0, 1, 1, 2);
    messageLayout->addWidget(durationLabel, 1, 0);
    messageLayout->addWidget(durationSpinBox, 1, 1);
    messageLayout->addWidget(durationWarningLabel, 1, 2, 1, 3);
    messageLayout->addWidget(titleLabel, 2, 0);
    messageLayout->addWidget(titleEdit, 2, 1, 1, 4);
    messageLayout->addWidget(bodyLabel, 3, 0);
    messageLayout->addWidget(bodyEdit, 3, 1, 2, 4);
    messageLayout->addWidget(showMessageButton, 5, 4);
    messageLayout->setColumnStretch(3, 1);
    messageLayout->setRowStretch(4, 1);
    messageGroupBox->setLayout(messageLayout);
}

void Setting::createTimeSettingGroupBox() {

    timeSettingGroupBox = new QGroupBox(tr("Time Setting"));

    restTimeLabel = new QLabel(tr("Rest time duration:"));

    restTimeSpinBox = new QSpinBox;
    restTimeSpinBox->setRange(5, 60);
    restTimeSpinBox->setSuffix(" m");
    restTimeSpinBox->setValue(this->getTime("rest"));

    seatedWorkTimeLabel = new QLabel(tr("Work time duration:"));

    seatedWorkTimeSpinBox = new QSpinBox;
    seatedWorkTimeSpinBox->setRange(5, 60);
    seatedWorkTimeSpinBox->setSuffix(" m");

    seatedWorkTimeSpinBox->setValue(this->getTime("seatedwork"));

    standWorkTimeLabel = new QLabel(tr("Stand time duration:"));

    standWorkTimeSpinBox = new QSpinBox;
    standWorkTimeSpinBox->setRange(5, 60);
    standWorkTimeSpinBox->setSuffix(" m");
    standWorkTimeSpinBox->setValue(1);

    QGridLayout *messageLayout = new QGridLayout;

    messageLayout->addWidget(restTimeLabel, 1, 0);
    messageLayout->addWidget(restTimeSpinBox, 1, 1);

    messageLayout->addWidget(seatedWorkTimeLabel, 2, 0);
    messageLayout->addWidget(seatedWorkTimeSpinBox, 2, 1);

    messageLayout->addWidget(standWorkTimeLabel, 3, 0);
    messageLayout->addWidget(standWorkTimeSpinBox, 3, 1);


    messageLayout->setColumnStretch(4, 1);
    messageLayout->setRowStretch(5, 1);
    timeSettingGroupBox->setLayout(messageLayout);
}

void Setting::createActions()
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

void Setting::createTrayIcon()
{
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(minimizeAction);
    trayIconMenu->addAction(maximizeAction);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);

}

#endif
