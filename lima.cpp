

#include <QString>
#include <QStandardPaths>
#include <QFileInfo>

#include "lima.h"

QString limactlPath()
{
    return QStandardPaths::findExecutable("limactl");
}
