/****************************************************************************
**
** Copyright (C) 2021 Anders F Björklund
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

#include "window.h"

#ifndef QT_NO_SYSTEMTRAYICON

#include <QAction>
#include <QComboBox>
#include <QCoreApplication>
#include <QCloseEvent>
#include <QDebug>
#include <QFont>
#include <QGroupBox>
#include <QLabel>
#include <QListView>
#include <QLineEdit>
#include <QMainWindow>
#include <QMenu>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QPushButton>
#include <QProcess>
#include <QString>
#include <QStandardPaths>
#include <QTemporaryDir>
#include <QTextEdit>
#include <QJsonDocument>
#include <QJsonObject>

#ifndef QT_NO_TERMWIDGET
#include <QApplication>
#include <QMainWindow>
#include "qtermwidget.h"
#endif

#ifndef QT_NO_SOURCEHIGHLITER
#include "qsourcehighliter.h"
using namespace QSourceHighlite;
#endif

//! [0]
Window::Window()
{
    iconComboBox = new QComboBox;
    iconComboBox->addItem(QIcon(":/images/tux.png"), tr("Tux"));
    createInstanceGroupBox();

    createActions();
    createTrayIcon();

    editDir = nullptr;
    editFile = nullptr;

    connect(refreshButton, &QAbstractButton::clicked, this, &Window::updateInstances);

    connect(shellButton, &QAbstractButton::clicked, this, &Window::shellConsole);
    connect(createButton, &QAbstractButton::clicked, this, &Window::createEditor);
    connect(iconComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &Window::setIcon);
    connect(startButton, &QAbstractButton::clicked, this, &Window::startInstance);
    connect(stopButton, &QAbstractButton::clicked, this, &Window::stopInstance);
    connect(removeButton, &QAbstractButton::clicked, this, &Window::removeInstance);
    connect(trayIcon, &QSystemTrayIcon::activated, this, &Window::iconActivated);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(instanceGroupBox);
    setLayout(mainLayout);

    iconComboBox->setCurrentIndex(0);
    trayIcon->show();

    setWindowTitle(tr("Lima"));
    setWindowIcon(*trayIconIcon);
    resize(400, 300);
}
//! [0]

//! [1]
void Window::setVisible(bool visible)
{
    minimizeAction->setEnabled(visible);
    maximizeAction->setEnabled(!isMaximized());
    restoreAction->setEnabled(isMaximized() || !visible);
    QDialog::setVisible(visible);
}
//! [1]

//! [2]
void Window::closeEvent(QCloseEvent *event)
{
#ifdef Q_OS_OSX
    if (!event->spontaneous() || !isVisible()) {
        return;
    }
#endif
    if (trayIcon->isVisible()) {
        QMessageBox::information(this, tr("Systray"),
                                 tr("The program will keep running in the "
                                    "system tray. To terminate the program, "
                                    "choose <b>Quit</b> in the context menu "
                                    "of the system tray entry."));
        hide();
        event->ignore();
    }
}
//! [2]

//! [3]
void Window::setIcon(int index)
{
    QIcon icon = iconComboBox->itemIcon(index);
    trayIcon->setIcon(icon);

    trayIcon->setToolTip(iconComboBox->itemText(index));
}
//! [3]

//! [4]
void Window::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
        iconComboBox->setCurrentIndex((iconComboBox->currentIndex() + 1) % iconComboBox->count());
        break;
    default:
        ;
    }
}
//! [4]

QString Window::selectedInstance()
{
    QModelIndex index = instanceListView->currentIndex();
    return index.data(Qt::DisplayRole).toString();
}

void Window::setSelectedInstance(QString instance)
{
    QAbstractItemModel *model = instanceListView->model();
    QModelIndex start = model->index(0, 0);
    QModelIndexList index = model->match(start, Qt::DisplayRole, instance);
    instanceListView->setCurrentIndex(index[0]);
}

void Window::shellConsole()
{
    QString instance = selectedInstance();
    QString program = QStandardPaths::findExecutable("limactl");
#ifndef QT_NO_TERMWIDGET
    QMainWindow *mainWindow = new QMainWindow();

    int startnow = 0; // set shell program first
    QTermWidget *console = new QTermWidget(startnow);
    console->setShellProgram(program);
    QStringList args = {"shell", instance};
    console->setArgs(args);
    console->startShellProgram();

    QObject::connect(console, SIGNAL(finished()), mainWindow, SLOT(close()));

    mainWindow->setWindowTitle(QString(tr("lima [%1]")).arg(instance));
    mainWindow->resize(640, 480);
    mainWindow->setCentralWidget(console);
    mainWindow->show();
#else
    QString terminal = qEnvironmentVariable("TERMINAL");
    if (terminal.isEmpty()) {
        terminal = "x-terminal-emulator";
        if (QStandardPaths::findExecutable(terminal).isEmpty()) {
            terminal = "xterm";
        }
    }

    QStringList arguments;
    arguments << "-e" << QString("%1 shell %2").arg(program).arg(instance);
    QProcess *process = new QProcess(this);
    process->start(QStandardPaths::findExecutable(terminal), arguments);
#endif
}

void Window::createEditor()
{
    editWindow = new QMainWindow();

    QLabel *label = new QLabel(tr("Name"));
    createName = new QLineEdit("default");

    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->addWidget(label);
    topLayout->addWidget(createName);

    createYAML = new QTextEdit();
    QFont font("monospace");
    font.setStyleHint(QFont::Monospace);
    font.setPointSize(10);
    createYAML->setFont(font);
#ifndef QT_NO_SOURCEHIGHLITER
    QSourceHighliter *highlighter = new QSourceHighliter(createYAML->document());
    highlighter->setCurrentLanguage(QSourceHighliter::CodeYAML);
#endif

    QString prefix = "/usr/local";
    QString examples = prefix + "/share/doc/lima/examples";
    QString defaultYAML = examples + "/default.yaml";
    QFile file(defaultYAML);
    if (file.open(QFile::ReadOnly | QIODevice::Text)) {
      QString content = QString::fromUtf8(file.readAll());
      createYAML->setPlainText(content);
      file.close();
    }

    QPushButton *cancelButton = new QPushButton(tr("Cancel"));
    QPushButton *okButton = new QPushButton(tr("Create"));

    connect(cancelButton, SIGNAL(clicked()), editWindow, SLOT(close()));
    connect(okButton, &QAbstractButton::clicked, this, &Window::createInstance);

    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout->addWidget(cancelButton);
    bottomLayout->addWidget(okButton);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(topLayout);
    layout->addWidget(createYAML);
    layout->addLayout(bottomLayout);

    QWidget *widget = new QWidget();
    widget->setLayout(layout);

    editWindow->setWindowTitle(tr("lima"));
    editWindow->resize(640, 480);
    editWindow->setCentralWidget(widget);
    editWindow->show();
}

bool Window::getProcessOutput(QStringList arguments, QString& text) {
    bool success;

    QString program = "limactl";

    QProcess *process = new QProcess(this);
    process->start(program, arguments);
    success = process->waitForFinished();
    if (success) {
        text = process->readAllStandardOutput();
    } else {
        qDebug() << process->readAllStandardError();
    }
    delete process;
    return success;
}

void InstanceModel::setInstances(const QStringList &strings) {
    beginResetModel();
    stringList = strings;
    endResetModel();
}

int InstanceModel::rowCount(const QModelIndex &) const
{
    return stringList.count();
}

QVariant InstanceModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= stringList.size())
        return QVariant();

    if (role == Qt::DisplayRole)
        return stringList.at(index.row());
    else
        return QVariant();
}

QVariant InstanceModel::headerData(int section, Qt::Orientation orientation,
                                     int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal)
        return QStringLiteral("Column %1").arg(section);
    else
        return QStringLiteral("Row %1").arg(section);
}

QStringList Window::getInstances()
{
    QStringList instances;
    QStringList arguments;
    arguments << "list" << "--json";
    QString text;
    bool success = getProcessOutput(arguments, text);
    QStringList lines;
    if (success) {
        lines = text.split("\n", QString::SkipEmptyParts);
    }
    for (int i = 0; i < lines.size(); i++) {
        QString line = lines.at(i);
        QJsonParseError error;
        QJsonDocument json = QJsonDocument::fromJson(line.toUtf8(), &error);
        if (json.isNull()) {
            qDebug() <<  error.errorString();
            continue;
        }
        if (json.isObject()) {
            QJsonObject instance = json.object();
            if (instance.contains("name")) {
                instances << instance["name"].toString();
            }
        }
    }
    return instances;
}

void Window::createInstanceGroupBox()
{
    instanceGroupBox = new QGroupBox(tr("Instances"));

    QStringList instances = getInstances();
    instanceModel = new InstanceModel(instances);

    instanceListView = new QListView();
    instanceListView->setModel(instanceModel);
    instanceListView->setSelectionMode(QAbstractItemView::SingleSelection);
    setSelectedInstance("default");

    refreshButton = new QPushButton(tr("Refresh"));
    refreshButton->setIcon(QIcon(":/images/reload.png"));

    shellButton = new QPushButton(tr("Shell"));
    shellButton->setIcon(QIcon(":/images/terminal.png"));
    createButton = new QPushButton(tr("Create"));
    startButton = new QPushButton(tr("Start"));
    stopButton = new QPushButton(tr("Stop"));
    removeButton = new QPushButton(tr("Remove"));

    QHBoxLayout *refreshButtonLayout = new QHBoxLayout;
    refreshButtonLayout->addStretch();
    refreshButtonLayout->addWidget(refreshButton);

    QHBoxLayout *instanceButtonLayout = new QHBoxLayout;
    instanceButtonLayout->addWidget(shellButton);
    instanceButtonLayout->addWidget(createButton);
    instanceButtonLayout->addWidget(startButton);
    instanceButtonLayout->addWidget(stopButton);
    instanceButtonLayout->addWidget(removeButton);

    QVBoxLayout *instanceLayout = new QVBoxLayout;
    instanceLayout->addLayout(refreshButtonLayout);
    instanceLayout->addWidget(instanceListView);
    instanceLayout->addLayout(instanceButtonLayout);
    instanceGroupBox->setLayout(instanceLayout);
}

void Window::updateInstances()
{
    QString instance = selectedInstance();
    instanceModel->setInstances(getInstances());
    setSelectedInstance(instance);
}

void Window::sendCommand(QString cmd)
{
    return sendCommand(QStringList(cmd));
}

void Window::sendCommand(QStringList arguments)
{
    QString program = "limactl";
    process = new QProcess(this);
    connect(process, SIGNAL(started()), this, SLOT(startedCommand()));
    connect(process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(finishedCommand(int, QProcess::ExitStatus)));
    process->start(program, arguments);
}

void Window::startedCommand()
{
    this->setCursor(Qt::WaitCursor);
}

void Window::finishedCommand(int code, QProcess::ExitStatus status)
{
    this->unsetCursor();
    if (status == QProcess::NormalExit && code != 0) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText(process->program() + " " + process->arguments().join(" "));
        msgBox.setInformativeText(process->readAllStandardError());
        msgBox.exec();
    }

    if (editFile) {
       editFile->remove();
       delete editFile;
       editFile = nullptr;
       delete editDir;
       editDir = nullptr;
    }
}

void Window::createInstance()
{
    QString name = createName->text();
    editDir = new QTemporaryDir;
    if (!editDir->isValid()) {
        QMessageBox::warning(this, tr("lima"), tr("Could not create temporary dir"));
        return;
    }
    QFile *temp = new QFile(editDir->path() + "/" + QString("%1.yaml").arg(name));
    if (temp->open(QFile::WriteOnly | QIODevice::Text)) {
      QString yaml = createYAML->toPlainText();
      temp->write(yaml.toUtf8());
      temp->close();
    }
    editFile = temp;
    editWindow->close();
    QStringList args = {"start", "--tty=false", editFile->fileName()};
    sendCommand(args);

    updateInstances();
}

void Window::startInstance()
{
    QString instance = selectedInstance();
    QStringList args = {"start", instance};
    sendCommand(args);
    //updateInstances();
}

void Window::stopInstance()
{
    QString instance = selectedInstance();
    QStringList args = {"stop", instance};
    sendCommand(args);
    //updateInstances();
}

bool Window::askConfirm(QString instance)
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("lima"));
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setText(tr("Remove an existing machine"));
    msgBox.setInformativeText(tr("Are you sure you want to remove '%1' ? This will delete all files for the instance.").arg(instance));
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    return msgBox.exec() == QMessageBox::Ok;
}

void Window::removeInstance()
{
    QString instance = selectedInstance();
    if (askConfirm(instance)) {
        QStringList args = {"rm", "--force", instance};
        sendCommand(args);
        updateInstances();
    }
}

void Window::createActions()
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

void Window::createTrayIcon()
{
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(minimizeAction);
    trayIconMenu->addAction(maximizeAction);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    trayIconIcon = new QIcon(":/images/tux.png");

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->setIcon(*trayIconIcon);
}

#endif
