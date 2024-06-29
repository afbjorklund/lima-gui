#ifndef LIMA_H
#define LIMA_H

extern QString defaultYAML();
extern QString defaultURL();

extern QStringList brewPaths();
extern QString limactlPath(const QStringList &paths = QStringList());

#endif // LIMA_H
