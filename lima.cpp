

#include <QString>
#include <QStandardPaths>
#include <QFileInfo>

#include "lima.h"

QString defaultYAML()
{
    return "examples/default.yaml";
}

QString limactlPath()
{
    QString program = QStandardPaths::findExecutable("limactl");
    if (program.isEmpty()) {
        QStringList paths = { "/usr/local/bin", "/opt/homebrew/bin",
                              "/home/linuxbrew/.linuxbrew/bin" };
        program = QStandardPaths::findExecutable("limactl", paths);
    }
    return program;
}
