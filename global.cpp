#include "global.h"

#include <QString>
#include <QTextStream>
#include <QDir>
#include <QProcess>
#include <QDirIterator>
#include <QFileInfo>
#include <QDebug>

#include <signal.h>

bool checkconfig(QString file) {
    QFile config(file);
    config.open(QIODevice::ReadWrite);
    QTextStream in (&config);
    const QString content = in.readAll();
    std::string contentstr = content.toStdString();
    if(contentstr.find("true") != std::string::npos) {
        return true;
    }
    else {
        return false;
    }
    config.close();
};

void setDefaultWorkDir() {
    QDir::setCurrent("/mnt/onboard/.adds/inkbox");
}

QString readFile(QString file) {
   if(QFile::exists(file)) {
       QFile fileToRead(file);
       fileToRead.open(QIODevice::ReadOnly);
       QTextStream in (&fileToRead);
       QString content = in.readAll();
       qDebug() << "File: " << file << " has strings: " << content;
       return content;
   }
   else {
       return NULL;
   }
}

QByteArray readFileBytes(QString file) {
   if(QFile::exists(file)) {
       QFile fileToRead(file);
       fileToRead.open(QIODevice::ReadOnly);
       QByteArray content = fileToRead.readAll();
       qDebug() << "File: " << file << " has bytes: " << content;
       return content;
   }
   else {
       return NULL;
   }
}

QString launchProcess(QString path, QStringList args) {
    QProcess *Process = new QProcess();
    Process->start(path, args);
    Process->waitForFinished();
    Process->deleteLater();
    return QString(Process->readAllStandardOutput());
}

int getPidByName(QString name) {
    QDirIterator appsDir("/proc", QDirIterator::NoIteratorFlags);
    while (appsDir.hasNext()) {
        QDir dir(appsDir.next());

        // This doesnt work, but its literally coppied code from inkbox...
        // QFile process = QFile(dir.path() + "/cmdline");
        QFile processPath;
        processPath.setFileName(dir.path() + "/cmdline");

        if(processPath.exists() == true) {
            processPath.open(QIODevice::ReadOnly);
            QTextStream stream(&processPath);
            if(stream.readLine().contains(name) == true) {
                processPath.close();
                qDebug() << "pid found: " << dir.dirName();
                return QVariant(dir.dirName()).toInt();
            }
            processPath.close();
        }
    }
    qDebug() << "Pid wasn't found";
    return NULL;
}

QVector<int> pidList;

void freezeApp(int pid) {
    kill(pid, SIGSTOP);
    qDebug() << "Freezing " << pid;
}

void unfreezeApp(int pid) {
    kill(pid, SIGCONT);
    qDebug() << "Unfreezing " << pid;
}
