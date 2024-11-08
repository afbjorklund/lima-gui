

#include <QString>
#include <QStandardPaths>
#include <QFileInfo>

#include "lima.h"

QString defaultYAML()
{
    return "templates/default.yaml";
}

QString defaultURL()
{
    return "https://github.com/lima-vm/lima/blob/master/templates/default.yaml";
}

QStringList brewPaths()
{
    return QStringList({ "/usr/local/bin", "/opt/homebrew/bin", "/home/linuxbrew/.linuxbrew/bin",
                         "/opt/local/bin" });
}

static QStringList standardPaths;

QString limactlPath(const QStringList &paths)
{
    QStringList searchPaths;
    if (paths.isEmpty()) {
        searchPaths = standardPaths;
    } else {
        standardPaths = paths;
        searchPaths = paths;
    }
    QString program = QStandardPaths::findExecutable("limactl", searchPaths);
    if (program.isEmpty()) {
        program = QStandardPaths::findExecutable("limactl", brewPaths());
    }
    return program;
}
