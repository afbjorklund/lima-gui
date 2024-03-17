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
#include "lima.h"

#ifndef QT_NO_SYSTEMTRAYICON

#include <QAction>
#include <QApplication>
#include <QCheckBox>
#include <QComboBox>
#include <QCoreApplication>
#include <QCloseEvent>
#include <QDebug>
#include <QFont>
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QTableView>
#include <QHeaderView>
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
#include <QFileDialog>
#include <QFileInfo>
#include <QRegularExpression>
#include <QDesktopServices>
#include <QUrl>

#ifndef QT_NO_TERMWIDGET
#include <QApplication>
#include <QMainWindow>
#include "qtermwidget.h"
#endif

#ifndef QT_NO_VNCCLIENT
#include "qvncclientwidget.h"
#endif

#ifndef QT_NO_SOURCEHIGHLITER
#include "qsourcehighliter.h"
using namespace QSourceHighlite;
#endif

#ifndef QT_NO_EMOTICONS
#include <QrwEmoticons/QrwEmoticons>
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

    connect(createButton, &QAbstractButton::clicked, this, &Window::createEditor);
    connect(quickButton, &QAbstractButton::clicked, this, &Window::quickInstance);
    connect(instanceListQuiet, &QCheckBox::stateChanged, this, &Window::updateQuiet);
    connect(aboutButton, &QAbstractButton::clicked, this, &Window::aboutProgram);
    connect(refreshButton, &QAbstractButton::clicked, this, &Window::updateInstances);

    connect(iconComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &Window::setIcon);

    connect(shellButton, &QAbstractButton::clicked, this, &Window::shellConsole);
    connect(displayButton, &QAbstractButton::clicked, this, &Window::displayWindow);
    connect(startButton, &QAbstractButton::clicked, this, &Window::startInstance);
    connect(stopButton, &QAbstractButton::clicked, this, &Window::stopInstance);
    connect(inspectButton, &QAbstractButton::clicked, this, &Window::inspectInstance);
    connect(removeButton, &QAbstractButton::clicked, this, &Window::removeInstance);

    connect(trayIcon, &QSystemTrayIcon::activated, this, &Window::iconActivated);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(instanceGroupBox);
    setLayout(mainLayout);

    iconComboBox->setCurrentIndex(0);
    trayIcon->show();

    setWindowTitle(tr("Lima"));
    setWindowIcon(*trayIconIcon);
    resize(600, 400);
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
    if (QSystemTrayIcon::isSystemTrayAvailable() && trayIcon->isVisible()) {
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
    default:;
    }
}
//! [4]

QString Window::selectedInstance()
{
    QAbstractItemModel *model = instanceListView->model();
    QModelIndex index = instanceListView->currentIndex();
    index = model->index(index.row(), 0); // get name
    QVariant variant = index.data(Qt::DisplayRole);
    if (variant.isNull()) {
        return QString();
    }
    return variant.toString();
}

void Window::setSelectedInstance(QString instance)
{
    QAbstractItemModel *model = instanceListView->model();
    QModelIndex start = model->index(0, 0);
    QModelIndexList index = model->match(start, Qt::DisplayRole, instance);
    if (index.size() == 0) {
        return;
    }
    instanceListView->setCurrentIndex(index[0]);
}

void Window::shellConsole()
{
    QString instance = selectedInstance();
    QString program = limactlPath();
#ifndef QT_NO_TERMWIDGET
    QMainWindow *mainWindow = new QMainWindow();

    int startnow = 0; // set shell program first
    QTermWidget *console = new QTermWidget(startnow);
    console->setShellProgram(program);
    QStringList args = { "shell", instance };
    console->setArgs(args);
    console->startShellProgram();

    QObject::connect(console, SIGNAL(finished()), mainWindow, SLOT(close()));

    mainWindow->setWindowTitle(QString(tr("lima [%1]")).arg(instance));
    mainWindow->resize(640, 480);
    mainWindow->setCentralWidget(console);
    mainWindow->show();
#elif defined(Q_OS_OSX)
    QString command = program + " shell " + instance;
    QStringList arguments = { "-e", "tell app \"Terminal\"",         "-e", "activate",
                              "-e", "do script \"" + command + "\"", "-e", "end tell" };
    QProcess *process = new QProcess(this);
    process->start("/usr/bin/osascript", arguments);
#else
    QString terminal = QString::fromLocal8Bit(qgetenv("TERMINAL"));
    QStringList flags;
    QString execute = "-e";
    if (terminal.isEmpty()) {
        terminal = "x-terminal-emulator";
        QString path = QStandardPaths::findExecutable(terminal);
        if (path.isEmpty()) {
            terminal = "xterm";
        }
        if (QDir(path).canonicalPath() == "/usr/bin/io.elementary.terminal") {
            flags << "-n"; // --new-window
            execute = "-x"; // --commandline
        }
    }

    QStringList arguments;
    arguments << flags << execute << program << "shell" << instance;
    QProcess *process = new QProcess(this);
    process->start(QStandardPaths::findExecutable(terminal), arguments);
#endif
}

void Window::displayWindow()
{
    QString inst = selectedInstance();
    Instance instance = getInstanceHash()[inst];
    QFile displayFile(instance.dir() + "/" + "vncdisplay");
    if (!displayFile.open(QFile::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("lima"), tr("Could not read vnc display"));
        return;
    }
    QString display = QString::fromUtf8(displayFile.readAll()).trimmed();
    QStringList parts = display.split(":");
    QString address = parts[0];
#ifndef QT_NO_VNCCLIENT
    int port = 5900 + parts[1].toInt();
#endif
    QFile passwordFile(instance.dir() + "/" + "vncpassword");
    if (!passwordFile.open(QFile::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("lima"), tr("Could not read vnc password"));
        return;
    }
    QString password = QString::fromUtf8(passwordFile.readAll()).trimmed();
#ifndef QT_NO_VNCCLIENT
    QMainWindow *mainWindow = new QMainWindow();

    QVNCClientWidget *client = new QVNCClientWidget();
    client->connectToVncServer(address, password, port);

    mainWindow->setWindowTitle(QString(tr("lima [%1]")).arg(inst));
    mainWindow->resize(1024, 768);
    mainWindow->setCentralWidget(client);
    client->startFrameBufferUpdate();
    mainWindow->show();
    client->tryRefreshScreen();
#else
    // gvncviewer
#endif
}

static QString getPrefix()
{
    QString prefix = "/usr/local";
    QString program = limactlPath();
    if (!program.isEmpty()) {
        QString bin = QFileInfo(program).dir().absolutePath();
        prefix = QFileInfo(bin).dir().absolutePath();
    }
    return prefix;
}

static void tooltipLink(const QString &link)
{
    if (!link.isEmpty())
        QToolTip::showText(QCursor::pos(), link);
    else
        QToolTip::hideText();
}

void Window::yamlEditor(QString instanceName, QString setString, QString yamlFile, bool create,
                        bool edit, bool start)
{
    editWindow = new QMainWindow();

    QLabel *label = new QLabel(tr("Name"));
    createName = new QLineEdit(instanceName);
    if (!create || !edit) {
        createName->setReadOnly(true);
    }
    QLabel *label2 = new QLabel(tr("Set"));
    label2->setEnabled(edit);
    createSet = new QLineEdit(setString);
    if (edit) {
        createSet->setPlaceholderText(".cpus = 4 | .memory = \"4GiB\"");
    }
    createSet->setToolTip("modify the template inplace, using yq syntax");
    createSet->setEnabled(edit);
    createHelp = new QPushButton(tr("Docs..."));
    createHelp->setIcon(QIcon(":/images/help.png"));

    connect(createHelp, &QAbstractButton::clicked, this, &Window::helpDocs);

    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->addWidget(label);
    topLayout->addWidget(createName);
    QHBoxLayout *topLayout2 = new QHBoxLayout;
    topLayout2->addWidget(label2);
    topLayout2->addWidget(createSet);
    topLayout2->addWidget(createHelp);

#ifndef QT_NO_EMOTICONS
    QrwEmoticonsTextEdit *textEdit = new QrwEmoticonsTextEdit(this);
    textEdit->emoticons()->setProvider("openmoji", false);
    createYAML = textEdit;
#else
    createYAML = new QTextEdit("lima");
#endif
    createYAML->setWordWrapMode(QTextOption::NoWrap);
    createYAML->setReadOnly(!edit);
    QFont font("monospace");
    font.setStyleHint(QFont::Monospace);
    font.setPointSize(10);
    createYAML->setFont(font);
#ifndef QT_NO_SOURCEHIGHLITER
    QSourceHighliter *highlighter = new QSourceHighliter(createYAML->document());
    highlighter->setCurrentLanguage(QSourceHighliter::CodeYAML);
#endif

    QFile file(yamlFile);
    if (file.open(QFile::ReadOnly | QIODevice::Text)) {
        QString content = QString::fromUtf8(file.readAll());
        createYAML->setPlainText(content);
        file.close();
    }

    QPushButton *cancelButton =
            !edit ? new QPushButton(tr("Close")) : new QPushButton(tr("Cancel"));
    QPushButton *loadButton = new QPushButton(tr("Load..."));
    loadButton->setToolTip(tr("Load YAML file"));
    QPushButton *saveButton = new QPushButton(tr("Save..."));
    saveButton->setToolTip(tr("Save YAML file"));
    QPushButton *okButton = create ? new QPushButton(tr("Create")) : new QPushButton(tr("Ok"));
    okButton->setDefault(true);

    connect(cancelButton, SIGNAL(clicked()), editWindow, SLOT(close()));
    connect(loadButton, &QAbstractButton::clicked, this, &Window::loadYAML);
    connect(saveButton, &QAbstractButton::clicked, this, &Window::saveYAML);
    connect(okButton, &QAbstractButton::clicked, this,
            create ? &Window::createInstance : &Window::updateInstance);

    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout->addWidget(cancelButton);
    if (edit) {
        bottomLayout->addWidget(loadButton);
        bottomLayout->addWidget(saveButton);
        bottomLayout->addWidget(okButton);
    }

    QLabel *label3 = new QLabel(tr("Search"));
    createFind = new QLineEdit;
    connect(createFind, &QLineEdit::returnPressed, this, &Window::findYAML);
    QPushButton *findButton = new QPushButton(tr("Find"));
    findButton->setIcon(QIcon(":/images/search.png"));
    connect(findButton, &QPushButton::clicked, this, &Window::findYAML);
    QLabel *label4 = new QLabel(tr("Start"));
    label4->setEnabled(edit);
    createStart = new QCheckBox;
    createStart->setEnabled(edit);
    createStart->setChecked(start);
    QHBoxLayout *bottomLayout2 = new QHBoxLayout;
    bottomLayout2->addWidget(label3);
    bottomLayout2->addWidget(createFind);
    bottomLayout2->addWidget(findButton);
    bottomLayout2->addWidget(label4);
    bottomLayout2->addWidget(createStart);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(topLayout);
    layout->addLayout(topLayout2);
    layout->addWidget(createYAML);
    layout->addLayout(bottomLayout2);
    layout->addLayout(bottomLayout);

    QWidget *widget = new QWidget();
    widget->setLayout(layout);

    editWindow->setWindowTitle(tr("lima"));
    editWindow->resize(640, 480);
    editWindow->setCentralWidget(widget);
    editWindow->show();
}

void Window::createEditor()
{
    createEditorSet("");
}

void Window::createEditorSet(QString set)
{
    QString directory = getPrefix() + "/share/doc/lima";
    yamlEditor("default", set, directory + "/" + defaultYAML(), true, true, true);
}

QWidget *Window::newExampleButton(QString name)
{
    Example example = getExamples()[name];
    QIcon icon(QPixmap(":/logos/" + example.logo()));
    QPushButton *button = new QPushButton;
    qreal size = 48 * devicePixelRatio();
    button->setIconSize(QSize(size, size));
    button->setIcon(icon);
    QString text = example.text();
    QLabel *textLabel = new QLabel(text);
    textLabel->setAlignment(Qt::AlignCenter);
    QString url = example.url();
    QLabel *urlLabel = new QLabel("<small><a href=\"" + url + "\">" + url + "</a></small>");
    urlLabel->setAlignment(Qt::AlignCenter);
    urlLabel->setOpenExternalLinks(true);
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(button);
    layout->addWidget(textLabel);
    layout->addWidget(urlLabel);
    connect(button, &QAbstractButton::clicked, this, &Window::quickCreate);
    button->setProperty("name", name);
    QWidget *widget = new QWidget();
    widget->setLayout(layout);
    return widget;
}

void Window::quickCreate()
{
    quickDialog->close();
    QString name = QObject::sender()->property("name").value<QString>();
    Example example = getExamples()[name];
    QString examples = getPrefix() + "/share/doc/lima/examples";
    QString exampleYAML = examples + "/" + example.yaml();
    if (quickPreview->isChecked()) {
        yamlEditor(example.name(), quickSetString(), exampleYAML, true, true,
                   quickStart->isChecked());
    } else {
        editWindow = new QMainWindow;
        readYAML(exampleYAML);
        createSet->setText(quickSetString());
        createStart->setChecked(quickStart->isChecked());
        createInstance();
    }
}

void Window::urlCreate()
{
    quickDialog->close();
    createInstanceURL();
}

void Window::advancedCreate()
{
    quickDialog->close();
    createEditorSet(quickSetString());
}

QString Window::quickSetString()
{
    QStringList args;
    QComboBox *vmType = quickDialog->findChild<QComboBox *>("vmType");
    if (vmType && vmType->property("changed").isValid()) {
        args << ".vmType = \"" + vmType->currentText() + "\"";
    }
    QComboBox *arch = quickDialog->findChild<QComboBox *>("arch");
    if (arch && arch->property("changed").isValid()) {
        args << ".arch = \"" + arch->currentText() + "\"";
    }
    QLineEdit *cpus = quickDialog->findChild<QLineEdit *>("cpus");
    if (cpus && cpus->property("changed").isValid()) {
        args << ".cpus= " + cpus->text();
    }
    QLineEdit *memory = quickDialog->findChild<QLineEdit *>("memory");
    if (memory && memory->property("changed").isValid()) {
        args << ".memory = \"" + memory->text() + "\"";
    }
    QLineEdit *disk = quickDialog->findChild<QLineEdit *>("disk");
    if (disk && disk->property("changed").isValid()) {
        args << ".disk = \"" + disk->text() + "\"";
    }
    QComboBox *mountType = quickDialog->findChild<QComboBox *>("mountType");
    if (mountType && mountType->property("changed").isValid()) {
        args << ".mountType = \"" + mountType->currentText() + "\"";
    }
    QComboBox *audiodev = quickDialog->findChild<QComboBox *>("audiodev");
    if (audiodev && audiodev->property("changed").isValid()) {
        args << ".audio.device = \"" + audiodev->currentText() + "\"";
    }
    QComboBox *display = quickDialog->findChild<QComboBox *>("display");
    if (display && display->property("changed").isValid()) {
        args << ".video.display = \"" + display->currentText() + "\"";
    }
    return args.join(" | ");
}

void Window::setChanged(const QString &)
{
    sender()->setProperty("changed", true);
}

void Window::quickInstance()
{
    quickDialog = new QDialog(this);
    quickDialog->setWindowTitle(tr("Quick Start"));
    quickDialog->setModal(true);

    QPushButton *cancelButton = new QPushButton(tr("Cancel"));
    connect(cancelButton, SIGNAL(clicked()), quickDialog, SLOT(close()));
    QPushButton *urlButton = new QPushButton(tr("URL..."));
    connect(urlButton, &QAbstractButton::clicked, this, &Window::urlCreate);
    urlButton->setToolTip(tr("Create from URL"));
    QPushButton *createButton = new QPushButton(tr("Create"));
    connect(createButton, &QAbstractButton::clicked, this, &Window::quickCreate);
    createButton->setProperty("name", "default");

    createName = new QLineEdit;
    createSet = new QLineEdit;
    createYAML = new QTextEdit;
    createStart = new QCheckBox;

    createURL = new QLineEdit(defaultURL());
    createURL->setFixedWidth(440);
    QFont font;
    font.setPointSize(9);
    createURL->setFont(font);

    QWidget *machineGroupBox = new QWidget();
    QHBoxLayout *machineLayout = new QHBoxLayout;
    machineLayout->addWidget(new QLabel(tr("VM Type:")));
    QComboBox *vmType = new QComboBox;
    vmType->addItem("qemu");
#ifdef Q_OS_MACOS
    vmType->addItem("vz");
#endif
    vmType->setObjectName("vmType");
    connect(vmType, &QComboBox::currentTextChanged, this, &Window::setChanged);
    machineLayout->addWidget(vmType);
    machineLayout->addWidget(new QLabel(tr("Arch:")));
    QComboBox *arch = new QComboBox;
    arch->addItem(tr("default"));
    arch->addItem("x86_64");
    arch->addItem("aarch64");
    arch->addItem("armv7l");
    arch->addItem("riscv64");
    arch->setObjectName("arch");
    connect(arch, &QComboBox::currentTextChanged, this, &Window::setChanged);
    machineLayout->addWidget(arch);
    machineLayout->addWidget(new QLabel(tr("CPUs:")));
    QLineEdit *cpus = new QLineEdit("4");
    cpus->setObjectName("cpus");
    connect(cpus, &QLineEdit::textChanged, this, &Window::setChanged);
    machineLayout->addWidget(cpus);
    machineLayout->addWidget(new QLabel(tr("Memory:")));
    QLineEdit *memory = new QLineEdit("4GiB");
    memory->setObjectName("memory");
    connect(memory, &QLineEdit::textChanged, this, &Window::setChanged);
    machineLayout->addWidget(memory);
    machineLayout->addWidget(new QLabel(tr("Disk:")));
    QLineEdit *disk = new QLineEdit("100GiB");
    disk->setObjectName("disk");
    connect(disk, &QLineEdit::textChanged, this, &Window::setChanged);
    machineLayout->addWidget(disk);
    quickStart = new QCheckBox("Start now", this);
    quickStart->setChecked(true);
    quickStart->setToolTip(tr("Start after create"));
    machineLayout->addWidget(quickStart);
    machineGroupBox->setLayout(machineLayout);

    QWidget *advancedGroupBox = new QWidget();
    QHBoxLayout *advancedLayout = new QHBoxLayout;
    QPushButton *advancedButton = new QPushButton(tr("Advanced..."));
    connect(advancedButton, &QAbstractButton::clicked, this, &Window::advancedCreate);
    advancedButton->setChecked(false);
    advancedButton->setToolTip(tr("Open YAML editor"));
    advancedLayout->addWidget(advancedButton);
    advancedLayout->addStretch();
    advancedLayout->addWidget(new QLabel(tr("Mount Type:")));
    QComboBox *mountType = new QComboBox;
    mountType->addItem("");
    mountType->addItem("reverse-sshfs");
    mountType->addItem("9p");
    mountType->addItem("virtiofs");
    mountType->setObjectName("mountType");
    connect(vmType, &QComboBox::currentTextChanged, this, &Window::setChanged);
    advancedLayout->addWidget(mountType);
    advancedLayout->addWidget(new QLabel(tr("Audio Device:")));
    QComboBox *audiodev = new QComboBox;
    audiodev->addItem(""); // default
    audiodev->addItem("none");
#ifdef Q_OS_MACOS
    audiodev->addItem("coreaudio");
#endif
#ifdef Q_OS_WIN
    audiodev->addItem("dsound");
#endif
#ifdef Q_OS_LINUX
    audiodev->addItem("alsa");
    audiodev->addItem("oss");
    audiodev->addItem("pa");
#endif
#ifdef Q_OS_MACOS
    audiodev->addItem("vz");
#endif
    audiodev->setObjectName("audiodev");
    advancedLayout->addWidget(audiodev);
    advancedLayout->addWidget(new QLabel(tr("Video Display:")));
    QComboBox *display = new QComboBox;
    display->addItem("none");
    display->addItem("default");
#ifdef Q_OS_MACOS
    display->addItem("cocoa");
#endif
    display->addItem("sdl");
#ifdef Q_OS_LINUX
    display->addItem("gtk");
#endif
    display->addItem("vnc");
    display->setObjectName("display");
    connect(display, &QComboBox::currentTextChanged, this, &Window::setChanged);
    advancedLayout->addWidget(display);
    advancedLayout->addStretch();
    quickPreview = new QCheckBox("Edit YAML", this);
    advancedLayout->addWidget(quickPreview);
    quickPreview->setToolTip(tr("Edit before start"));
    advancedGroupBox->setLayout(advancedLayout);

    QGroupBox *distroGroupBox = new QGroupBox(tr("Linux Distributions"));
    QHBoxLayout *distroLayout = new QHBoxLayout;
    distroLayout->addWidget(newExampleButton("alpine"));
    distroLayout->addWidget(newExampleButton("ubuntu"));
    distroLayout->addWidget(newExampleButton("fedora"));
    distroLayout->addWidget(newExampleButton("archlinux"));
    distroGroupBox->setLayout(distroLayout);

    QGroupBox *engineGroupBox = new QGroupBox(tr("Container Engines"));
    QHBoxLayout *engineLayout = new QHBoxLayout;
    engineLayout->addWidget(newExampleButton("default"));
    engineLayout->addWidget(newExampleButton("docker"));
    engineLayout->addWidget(newExampleButton("podman"));
    engineLayout->addWidget(newExampleButton("apptainer"));
    engineGroupBox->setLayout(engineLayout);

    QGroupBox *orchestratorGroupBox = new QGroupBox(tr("Container Orchestration"));
    QHBoxLayout *orchestratorLayout = new QHBoxLayout;
    orchestratorLayout->addWidget(newExampleButton("k3s"));
    orchestratorLayout->addWidget(newExampleButton("k8s"));
    orchestratorLayout->addWidget(newExampleButton("nomad"));
    orchestratorLayout->addWidget(newExampleButton("faasd"));
    orchestratorGroupBox->setLayout(orchestratorLayout);

    QString url = "https://lima-vm.io/docs/templates/";
    QLabel *moreExamplesAvailable =
            new QLabel(tr("There are more <a href=\"%1\">templates</a> available. "
                          "Use the \"Load\" button (under Advanced), or \"URL\" below, "
                          "to use them.<br>")
                               .arg(url));
    moreExamplesAvailable->setOpenExternalLinks(true);
    connect(moreExamplesAvailable, &QLabel::linkHovered, this, &tooltipLink);

    QVBoxLayout *topLayout = new QVBoxLayout;
    topLayout->addWidget(machineGroupBox);
    topLayout->addWidget(advancedGroupBox);
    topLayout->addWidget(distroGroupBox);
    topLayout->addWidget(engineGroupBox);
    topLayout->addWidget(orchestratorGroupBox);
    topLayout->addWidget(moreExamplesAvailable);

    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout->addWidget(cancelButton);
    bottomLayout->addStretch();
    bottomLayout->addWidget(urlButton);
    bottomLayout->addWidget(createURL);
    bottomLayout->addStretch();
    bottomLayout->addWidget(createButton);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(topLayout);
    layout->addLayout(bottomLayout);

    quickDialog->resize(640, 480);
    quickDialog->setLayout(layout);
    createButton->setDefault(true);
    quickDialog->exec();
}

bool Window::getProcessOutput(QStringList arguments, QString &text)
{
    bool success;

    QString program = limactlPath();

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

QString Window::getVersion()
{
    QString program = limactlPath();
    if (!program.isEmpty()) {
        QStringList arguments;
        arguments << "--version";
        QString text;
        bool success = getProcessOutput(arguments, text);
        if (success) {
            text.replace("limactl version", "");
            return text.trimmed();
        }
    }
    return "N/A";
}

QStringList splitLines(QString text)
{
    QStringList lines;
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    lines = text.split("\n", Qt::SkipEmptyParts);
#else
    lines = text.split("\n", QString::SkipEmptyParts);
#endif
    return lines;
}

InstanceList Window::getInstances()
{
    InstanceList instances;
    QStringList arguments;
    arguments << "list"
              << "--json";
    QString text;
    bool success = getProcessOutput(arguments, text);
    QStringList lines;
    if (success) {
        lines = splitLines(text);
    }
    for (int i = 0; i < lines.size(); i++) {
        QString line = lines.at(i);
        QJsonParseError error;
        QJsonDocument json = QJsonDocument::fromJson(line.toUtf8(), &error);
        if (json.isNull()) {
            qDebug() << error.errorString();
            continue;
        }
        if (json.isObject()) {
            Instance instance(json.object());
            if (instance.name().isEmpty()) {
                continue;
            }
            instances << instance;
        }
    }
    return instances;
}

InstanceHash Window::getInstanceHash()
{
    InstanceList instances = getInstances();
    InstanceHash instanceHash;
    for (int i = 0; i < instances.size(); i++) {
        Instance instance = instances.at(i);
        instanceHash[instance.name()] = instance;
    }
    return instanceHash;
}

void Window::createInstanceGroupBox()
{
    instanceGroupBox = new QGroupBox(tr("Instances"));

    InstanceList instances = getInstances();
    instanceModel = new InstanceModel(instances);

    instanceListView = new QTableView();
    instanceListView->setModel(instanceModel);
    instanceListView->setSelectionMode(QAbstractItemView::SingleSelection);
    instanceListView->setSelectionBehavior(QAbstractItemView::SelectRows);
    instanceListView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    instanceListView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    instanceListView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    instanceListView->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    instanceListView->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    instanceListView->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
    setSelectedInstance("default");

    connect(instanceListView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(updateButtons()));
    connect(instanceListView, SIGNAL(doubleClicked(const QModelIndex &)), this,
            SLOT(inspectInstance()));

    instanceListQuiet = new QCheckBox(tr("Quiet"));
    instanceListQuiet->setToolTip(tr("Only show names"));

    createButton = new QPushButton(tr("Create"));
    createButton->setIcon(QIcon(":/images/add.png"));
    quickButton = new QPushButton(tr("Quick"));
    quickButton->setIcon(QIcon(":/images/quick.png"));
    aboutButton = new QPushButton(tr("About"));
    aboutButton->setIcon(QIcon(":/images/info.png"));
    refreshButton = new QPushButton(tr("Refresh"));
    refreshButton->setIcon(QIcon(":/images/reload.png"));

    shellButton = new QPushButton(tr("Shell"));
    shellButton->setIcon(QIcon(":/images/terminal.png"));
    displayButton = new QPushButton(tr("Display"));
    displayButton->setIcon(QIcon(":/images/desktopshare.png"));
    startButton = new QPushButton(tr("Start"));
    startButton->setIcon(QIcon(":/images/player_start.png"));
    stopButton = new QPushButton(tr("Stop"));
    stopButton->setIcon(QIcon(":/images/player_stop.png"));
    inspectButton = new QPushButton(tr("Inspect"));
    inspectButton->setIcon(QIcon(":/images/advanced.png"));
    removeButton = new QPushButton(tr("Remove"));
    removeButton->setIcon(QIcon(":/images/remove.png"));

    updateButtons();

    QHBoxLayout *refreshButtonLayout = new QHBoxLayout;
    refreshButtonLayout->addWidget(createButton);
    refreshButtonLayout->addWidget(quickButton);
    refreshButtonLayout->addStretch();
    refreshButtonLayout->addWidget(instanceListQuiet);
    refreshButtonLayout->addStretch();
    refreshButtonLayout->addWidget(aboutButton);
    refreshButtonLayout->addWidget(refreshButton);

    QHBoxLayout *instanceButtonLayout = new QHBoxLayout;
    instanceButtonLayout->addWidget(shellButton);
    instanceButtonLayout->addWidget(displayButton);
    instanceButtonLayout->addWidget(startButton);
    instanceButtonLayout->addWidget(stopButton);
    instanceButtonLayout->addWidget(inspectButton);
    instanceButtonLayout->addWidget(removeButton);

    QVBoxLayout *instanceLayout = new QVBoxLayout;
    instanceLayout->addLayout(refreshButtonLayout);
    instanceLayout->addWidget(instanceListView);
    instanceLayout->addLayout(instanceButtonLayout);
    instanceGroupBox->setLayout(instanceLayout);
}

void Window::updateButtons()
{
    QString inst = selectedInstance();
    if (inst.isEmpty()) {
        shellButton->setEnabled(false);
        displayButton->setEnabled(false);
        startButton->setEnabled(false);
        stopButton->setEnabled(false);
        inspectButton->setEnabled(false);
        removeButton->setEnabled(false);
        return;
    }
    Instance instance = getInstanceHash()[inst];
    if (instance.status() == "Running") {
        shellButton->setEnabled(true);
        shellButton->setToolTip("");
        bool hasDisplay = instance.videoDisplay() != "none";
        displayButton->setEnabled(hasDisplay);
        displayButton->setToolTip(hasDisplay ? "" : tr("Needs VNC enabled"));
        startButton->setEnabled(false);
        stopButton->setEnabled(true);
        inspectButton->setEnabled(true);
        removeButton->setEnabled(false);
    } else if (instance.status() == "Stopped") {
        shellButton->setEnabled(false);
        shellButton->setToolTip(tr("Needs to be running"));
        displayButton->setEnabled(false);
        displayButton->setToolTip(tr("Needs to be running"));
        startButton->setEnabled(true);
        stopButton->setEnabled(false);
        inspectButton->setEnabled(true);
        removeButton->setEnabled(true);
    }
}

void Window::aboutProgram()
{
    QString title = tr("Lima: Linux Machines");
    QString url = "https://lima-vm.io";
    QString text = "<h1>" + title + "</h1>";
    text += "<a align=\"right\" href=\"" + url + "\">" + url + "</a>";
    text += tr("<p>Use <b>Create</b> (or <b>Quick</b>) to create a new instance of Lima.</p>");
    text += tr("<p>To access an instance, use <b>Shell</b> or the "
               "<tt>lima</tt> command.</p>");
    text += tr("<p>Use <b>Display</b> to connect to the instance's graphical display.</p>");
    text += tr("<p>This requires that the VNC display has been enabled for it.</p>");
    QMessageBox::about(this, tr("Lima") + " version " + getVersion(), text);
}

void Window::updateInstances()
{
    QString instance = selectedInstance();
    instanceModel->setInstances(getInstances());
    setSelectedInstance(instance);
    updateQuiet(instanceListQuiet->checkState());
}

void Window::updateQuiet(int state)
{
    bool hide = (state == Qt::Checked);
    instanceModel->setQuiet(hide);
    for (int i = 1; i < instanceModel->columnCount(); i++) {
        instanceListView->setColumnHidden(i, hide);
    }
    instanceListView->update();
}

void Window::sendCommand(QString cmd)
{
    return sendCommand(QStringList(cmd));
}

void Window::sendCommand(QStringList arguments)
{
    QString program = limactlPath();
    process = new QProcess(this);
#ifdef Q_OS_OSX
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    // the macOS PATH does not inherit from the terminal PATH, add brew
    env.insert("PATH", brewPaths().join(":") + ":" + env.value("PATH"));
    process->setProcessEnvironment(env);
#endif
    connect(process, SIGNAL(started()), this, SLOT(startedCommand()));
    connect(process, SIGNAL(finished(int, QProcess::ExitStatus)), this,
            SLOT(finishedCommand(int, QProcess::ExitStatus)));
    process->start(program, arguments);
}

void Window::startedCommand()
{
    this->setCursor(Qt::WaitCursor);
}

void Window::finishedCommand(int code, QProcess::ExitStatus status)
{
    this->unsetCursor();
    QString command = process->program() + " " + process->arguments().join(" ");
    if (status == QProcess::NormalExit && code != 0) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText("<b>" + command + "</b>");
        msgBox.setInformativeText(process->readAllStandardError());
        msgBox.exec();
    } else {
        QByteArray output = process->readAllStandardOutput();
        if (output.length() > 0) {
            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setText("<b>" + command + "</b>");
            msgBox.setInformativeText(output);
            msgBox.exec();
        }
    }

    if (editFile) {
        editFile->remove();
        delete editFile;
        editFile = nullptr;
    }
    if (editDir) {
        delete editDir;
        editDir = nullptr;
    }
}

QString Window::outputCommand(QStringList arguments)
{
    QString program = limactlPath();
    process = new QProcess(this);
    process->start(program, arguments);
    process->waitForFinished();
    QByteArray out = process->readAllStandardOutput();
    return QString(out);
}

QString Window::shellCommand(QString name, QStringList arguments)
{
    QStringList args = { "shell", name };
    args.append(arguments);
    return outputCommand(args);
}

void Window::helpDocs()
{
    QString link = "https://lima-vm.io/docs/";
    QDesktopServices::openUrl(QUrl(link));
}

void Window::findYAML()
{
    createYAML->find(createFind->text());
}

void Window::loadYAML()
{
    QString examples = getPrefix() + "/share/doc/lima/examples";

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open YAML"), examples,
                                                    tr("YAML Files (*.yaml)"));
    if (fileName.isEmpty()) {
        return;
    }
    readYAML(fileName);
}

void Window::readYAML(QString fileName)
{
    QString baseName = QFileInfo(fileName).baseName();
    createName->setText(baseName.replace(".yaml", ""));
    QFile file(fileName);
    if (file.open(QFile::ReadOnly | QIODevice::Text)) {
        QString content = QString::fromUtf8(file.readAll());
        createYAML->setPlainText(content);
        file.close();
    }
}

void Window::saveYAML()
{
    QString baseName = createName->text() + ".yaml";
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save YAML"), baseName,
                                                    tr("YAML Files (*.yaml)"));
    if (fileName.isEmpty()) {
        return;
    }
    writeYAML(fileName);
}

void Window::writeYAML(QString fileName)
{
    QFile file(fileName);
    if (file.open(QFile::WriteOnly | QIODevice::Text)) {
        QString yaml = createYAML->toPlainText();
        file.write(yaml.toUtf8());
        file.close();
    }
}

QFile *Window::validateYAML(QString name)
{
    editDir = new QTemporaryDir;
    if (!editDir->isValid()) {
        QMessageBox::warning(this, tr("lima"), tr("Could not create temporary dir"));
        return 0;
    }
    QFile *temp = new QFile(editDir->path() + "/" + QString("%1.yaml").arg(name));
    if (temp->open(QFile::WriteOnly | QIODevice::Text)) {
        QString yaml = createYAML->toPlainText();
        temp->write(yaml.toUtf8());
        temp->close();
    } else {
        QMessageBox::warning(this, tr("lima"), tr("Could not create temporary file"));
        return 0;
    }
    QProcess process(this);
    QString program = limactlPath();
    process.start(program, { "validate", temp->fileName() });
    bool success = process.waitForFinished();
    if (success) {
        if (process.exitStatus() == QProcess::NormalExit && process.exitCode() != 0) {
            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.setText(tr("Validation failed!"));
            msgBox.setInformativeText(process.readAllStandardError());
            msgBox.exec();
            delete temp;
            return 0;
        }
    }
    return temp;
}

void Window::createInstance()
{
    QString name = createName->text();
    QString set = createSet->text();
    editFile = validateYAML(name);
    if (!editFile)
        return;
    editWindow->close();
    bool start = createStart->isChecked();
    QStringList args;
    if (start) {
        args << "start";
    } else {
        args << "create";
    }
    args << "--tty=false";
    if (!set.isEmpty()) {
        args << "--set";
        args << set;
    }
    args << editFile->fileName();
    sendCommand(args);

    updateInstances();
}

void Window::createInstanceURL()
{
    QString url = createURL->text();
    QString set = createSet->text();
    if (url.startsWith("https://github.com")) {
        // Get the "raw" YAML, and not the prettified HTTP
        url = url.replace("github.com", "raw.githubusercontent.com");
        url = url.replace("blob/", "");
    }
    bool start = createStart->isChecked();
    QStringList args;
    if (start) {
        args << "start";
    } else {
        args << "create";
    }
    args << "--tty=false";
    if (!set.isEmpty()) {
        args << "--set";
        args << set;
    }
    args << url;
    sendCommand(args);

    updateInstances();
}

void Window::updateInstance()
{
    QString name = createName->text();
    QString set = createSet->text();
    editFile = validateYAML(name);
    if (!editFile)
        return;
    editWindow->close();
    bool start = createStart->isChecked();
    Instance instance = getInstanceHash()[name];
    QString yamlFile = instance.dir() + "/" + "lima.yaml";
    QFile(yamlFile).remove();
    bool ok = editFile->rename(yamlFile);
    delete editFile;
    editFile = nullptr;
    if (ok && !set.isEmpty()) {
        QStringList args = { "edit", "--tty=false" };
        args << "--set";
        args << set;
        args << name;
        outputCommand(args);
    } else if (!ok) {
        QMessageBox::warning(this, tr("lima"), tr("Failed to update") + " " + yamlFile);
    }
    if (start) {
        QStringList args2 = { "start", "--tty=false" };
        args2 << name;
        sendCommand(args2);
    }
}

void Window::startInstance()
{
    QString instance = selectedInstance();
    QStringList args = { "start", instance };
    sendCommand(args);
    // updateInstances();
}

void Window::stopInstance()
{
    QString instance = selectedInstance();
    QStringList args = { "stop", instance };
    sendCommand(args);
    // updateInstances();
}

void Window::inspectInstance()
{
    QString name = selectedInstance();
    if (name.isEmpty())
        return;
    Instance instance = getInstanceHash()[name];
    if (instance.name().isEmpty())
        return;

    QString kernel = tr("N/A");
    QString pretty = tr("N/A");
    QString uptime = tr("N/A");
    QString id;
    QString var;
    QString ver;
    QString logo;
    bool running = instance.status() == "Running";
    if (running) {
        // uname
        QStringList args = { "uname", "-sr" }; // -a == -snrvmpio
        kernel = shellCommand(name, args).trimmed();
        // uptime
        QString out = shellCommand(name, QStringList("uptime"));
        QRegularExpression re("^(.*) up (.*?)(, +\\d+ users?)*, +load average\\: (.*)$");
        QRegularExpressionMatch m = re.match(out);
        if (m.hasMatch()) {
            uptime = m.captured(2);
        }
        // os-release
        args = QStringList({ "cat", "/etc/os-release" });
        QString release = shellCommand(name, args);
        QStringList lines = release.split("\n");
        QStringList match = lines.filter("PRETTY_NAME=");
        pretty = match.length() > 0 ? match[0].replace("PRETTY_NAME=", "") : "";
        pretty = pretty.replace("\"", "");
        match = lines.filter("VARIANT=");
        QString variant = match.length() > 0 ? match[0].replace("VARIANT=", "") : "";
        variant = variant.replace("\"", "");
        if (!variant.isEmpty() && !pretty.contains(variant)) {
            pretty += " ( " + variant + ")";
        }
        match = lines.filter("VERSION_CODENAME=");
        QString codename = match.length() > 0 ? match[0].replace("VERSION_CODENAME=", "") : "";
        codename = codename.replace("\"", "");
        if (!codename.isEmpty() && !pretty.contains(codename)) {
            pretty += " (" + codename + ")";
        }
        match = lines.filter(QRegularExpression("^ID="));
        id = match.length() > 0 ? match[0].replace("ID=", "").replace("\"", "") : "";
        match = lines.filter(QRegularExpression("^VARIANT_ID="));
        var = match.length() > 0 ? match[0].replace("VARIANT_ID=", "").replace("\"", "") : "";
        match = lines.filter(QRegularExpression("^VERSION_ID="));
        ver = match.length() > 0 ? match[0].replace("VERSION_ID=", "").replace("\"", "") : "";
        match = lines.filter(QRegularExpression("^NAME="));
        QString distro = match.length() > 0 ? match[0].replace("NAME=", "") : "";
        distro = distro.replace("\"", "");
        if (id == "centos" && distro == "CentOS Stream") { // != "CentOS Linux"
            id = "centos-stream";
        }
        if (id == "fedora" && variant == "CoreOS") { // != "Cloud Edition"
            id = "fedora-coreos";
        }
        Example example = getExamples()[id];
        logo = example.logo();
    }

    QDialog *dialog = new QDialog(this);
    dialog->setMinimumWidth(300);
    dialog->setWindowTitle(name);
    dialog->setModal(true);
    QPushButton *button = new QPushButton(tr("Close"));
    button->setDefault(true);
    connect(button, SIGNAL(clicked()), dialog, SLOT(close()));
    QGroupBox *instanceBox = new QGroupBox(tr("Instance"));
    QFormLayout *form1 = new QFormLayout;
    form1->addRow(new QLabel(tr("Name:")), new QLabel(instance.name()));
    form1->addRow(new QLabel(tr("Status:")), new QLabel(instance.status()));
    QString sshAddress = QString("%1:%2").arg(instance.sshAddress()).arg(instance.sshLocalPort());
    QLabel *sshLabel = new QLabel(sshAddress);
    if (running) {
        sshLabel->setText(QString("<a href=\"ssh://%1\">%2</a>").arg(sshAddress, sshAddress));
        connect(sshLabel, &QLabel::linkActivated, this, &Window::shellConsole);
        connect(sshLabel, &QLabel::linkHovered, this, &tooltipLink);
    }
    form1->addRow(new QLabel(tr("SSH:")), sshLabel);
    form1->addRow(new QLabel(tr("VM Type:")), new QLabel(instance.vmType()));
    form1->addRow(new QLabel(tr("Arch:")), new QLabel(instance.arch()));
    form1->addRow(new QLabel(tr("CPUs:")), new QLabel(instance.strCpus()));
    form1->addRow(new QLabel(tr("Memory:")), new QLabel(instance.strMemory()));
    form1->addRow(new QLabel(tr("Disk:")), new QLabel(instance.strDisk()));
    QString homeDir = QDir::homePath();
    QString instDir = instance.dir().replace(homeDir, "~");
    QLabel *dirLabel =
            new QLabel(QString("<a href=\"file://%1\">%2</a>").arg(instance.dir(), instDir));
    dirLabel->setOpenExternalLinks(true);
    connect(dirLabel, &QLabel::linkHovered, this, &tooltipLink);
    form1->addRow(new QLabel(tr("Dir:")), dirLabel);
    instanceBox->setLayout(form1);
    QGroupBox *advancedBox = new QGroupBox(tr("Advanced"));
    QFormLayout *form2 = new QFormLayout;
    form2->addRow(new QLabel(tr("Mount Type:")), new QLabel(instance.mountType()));
    form2->addRow(new QLabel(tr("Audio Device:")), new QLabel(instance.audioDevice()));
    form2->addRow(new QLabel(tr("Video Display:")), new QLabel(instance.videoDisplay()));
    advancedBox->setLayout(form2);
    QGroupBox *limayamlBox = new QGroupBox(tr("lima.yaml"));
    QHBoxLayout *limayamlButtonLayout = new QHBoxLayout;
    QPushButton *viewButton = new QPushButton(tr("View"));
    limayamlButtonLayout->addWidget(viewButton);
    QPushButton *editButton = new QPushButton(tr("Edit"));
    limayamlButtonLayout->addWidget(editButton);
    limayamlButtonLayout->addStretch();
    QPushButton *messageButton = new QPushButton(tr("Message"));
    limayamlButtonLayout->addWidget(messageButton);
    if (instance.status() == "Running") {
        editButton->setEnabled(false);
        editButton->setToolTip(tr("Can't edit running instance"));
        bool hasMessage = instance.config().contains("message");
        messageButton->setEnabled(hasMessage);
        messageButton->setToolTip(hasMessage ? "" : tr("No message found"));
    } else if (instance.status() == "Stopped") {
        editButton->setEnabled(true);
        messageButton->setEnabled(false);
    }
    limayamlBox->setLayout(limayamlButtonLayout);
    connect(viewButton, &QAbstractButton::clicked, this, &Window::viewInstance);
    connect(viewButton, SIGNAL(clicked()), dialog, SLOT(close()));
    connect(editButton, &QAbstractButton::clicked, this, &Window::editInstance);
    connect(editButton, SIGNAL(clicked()), dialog, SLOT(close()));
    connect(messageButton, &QAbstractButton::clicked, this, &Window::messageInstance);
    QGroupBox *systemBox = new QGroupBox(tr("System"));
    QFormLayout *form3 = new QFormLayout;
    if (!logo.isEmpty()) {
        QLabel *logoLabel = new QLabel;
        QPixmap *pixmap = new QPixmap(QString(":/logos/" + logo));
        if (!pixmap->isNull()) {
            QPixmap scaled = pixmap->scaledToWidth(128, Qt::SmoothTransformation);
            logoLabel->setPixmap(scaled);
            form3->addRow(new QWidget, logoLabel);
        }
    }
    form3->addRow(new QLabel(tr("Release:")), new QLabel(pretty));
    form3->addRow(new QLabel(tr("Kernel:")), new QLabel(kernel));
    form3->addRow(new QLabel(tr("Uptime:")), new QLabel(uptime));
    systemBox->setLayout(form3);
    systemBox->setEnabled(running);
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(instanceBox);
    layout->addWidget(advancedBox);
    layout->addWidget(limayamlBox);
    layout->addWidget(systemBox);
    layout->addStretch();
    layout->addWidget(button);
    dialog->setLayout(layout);
    dialog->exec();
}

void Window::viewInstance()
{
    QString name = selectedInstance();
    Instance instance = getInstanceHash()[name];
    QString yamlFile = instance.dir() + "/" + "lima.yaml";
    yamlEditor(instance.name(), "", yamlFile, false, false, false);
}

void Window::editInstance()
{
    QString name = selectedInstance();
    Instance instance = getInstanceHash()[name];
    QString yamlFile = instance.dir() + "/" + "lima.yaml";
    yamlEditor(instance.name(), "", yamlFile, false, true, true);
}

void Window::messageInstance()
{
    QString name = selectedInstance();
    QProcess process(this);
    QString program = limactlPath();
    // limactl list will automagically expand all variables in the message
    process.start(program, { "list", "--format", "{{.Message}}", name });
    bool success = process.waitForFinished();
    if (success) {
        if (process.exitStatus() == QProcess::NormalExit && process.exitCode() == 0) {
            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setText("<b>" + name + "</b>");
            QString message = process.readAllStandardOutput();
            msgBox.setInformativeText(Qt::convertFromPlainText(message));
            msgBox.exec();
        }
    }
}

bool Window::askConfirm(QString instance)
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("lima"));
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setText(tr("Remove an existing instance"));
    msgBox.setInformativeText(tr("Are you sure you want to remove '%1' ? "
                                 "This will delete all files for the instance.")
                                      .arg(instance));
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    return msgBox.exec() == QMessageBox::Ok;
}

void Window::removeInstance()
{
    QString instance = selectedInstance();
    if (askConfirm(instance)) {
        QStringList args = { "rm", "--force", instance };
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

    aboutAction = new QAction(tr("About"), this);
    connect(aboutAction, &QAction::triggered, this, &Window::aboutProgram);

    aboutQtAction = new QAction(tr("About &Qt"), this);
    connect(aboutQtAction, &QAction::triggered, this, &QApplication::aboutQt);

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
    trayIconMenu->addAction(aboutAction);
    trayIconMenu->addAction(aboutQtAction);
    trayIconMenu->addAction(quitAction);

    trayIconIcon = new QIcon(":/images/tux.png");

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->setIcon(*trayIconIcon);
}

#endif
