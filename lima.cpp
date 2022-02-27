

#include <QString>
#include <QStandardPaths>
#include <QFileInfo>

#include "lima.h"

QString defaultYAML()
{
    return "examples/default.yaml";
}

QString defaultURL()
{
    return "https://github.com/lima-vm/lima/blob/master/examples/default.yaml";
}

QStringList brewPaths()
{
    return QStringList({ "/usr/local/bin", "/opt/homebrew/bin", "/home/linuxbrew/.linuxbrew/bin" });
}

QString limactlPath()
{
    QString program = QStandardPaths::findExecutable("limactl");
    if (program.isEmpty()) {
        QStringList paths = brewPaths();
        program = QStandardPaths::findExecutable("limactl", paths);
    }
    return program;
}
