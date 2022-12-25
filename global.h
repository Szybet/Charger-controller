#ifndef GLOBAL_H
#define GLOBAL_H

// https://stackoverflow.com/questions/23922319/global-variables-in-qt-5-3

#include <QtGlobal>

QT_BEGIN_NAMESPACE
class QString;
class QFile;
class QStringList;
QT_END_NAMESPACE

extern bool checkconfig(QString file);

extern void setDefaultWorkDir();

extern QString readFile(QString file);

extern int getPidByName(QString name);

extern QString launchProcess(QString path, QStringList args);

extern QVector<int> pidList;

extern void freezeApp(int pid);

extern void unfreezeApp(int pid);

extern QByteArray readFileBytes(QString file);

#endif // GLOBAL_H
