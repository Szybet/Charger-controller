#include "main_menu.h"
#include "ui_main_menu.h"
#include "global.h"

#include <QTimer>
#include <QDebug>
#include <QProcess>
#include <QMessageBox>
#include <QFile>
#include <QThread>
#include <QDir>
#include <QDebug>
#include <QScreen>

#include <QApplication>

main_menu::main_menu(QDialog *parent)
    : QDialog(parent)
    , ui(new Ui::main_menu)
{
    ui->setupUi(this);
    qDebug() << "Setting up charger manager";

    // Stylesheet
    QApplication::setStyle("windows");
    QFile stylesheetFile("/mnt/onboard/.adds/inkbox/eink.qss");
    stylesheetFile.open(QFile::ReadOnly);
    this->setStyleSheet(stylesheetFile.readAll());
    stylesheetFile.close();

    // items
    ui->progressBar->setFormat("");
    ui->label->setStyleSheet("font-size: 11pt;");

    ui->label_2->setStyleSheet("font-size: 11pt;");
    ui->labelSeconds->setStyleSheet("font-size: 11pt;");

    ui->StopButton->setStyleSheet("font-size: 7pt;");
    ui->cancelButton->setStyleSheet("font-size: 7pt;");
    ui->AcceptButton->setStyleSheet("font-size: 7pt;");

    QSize size = ui->plusButton->sizeHint(); // get the size hint of the button
    ui->plusButton->setFixedSize(size.height(), size.height());
    ui->minusButton->setFixedSize(size.height(), size.height());

    ui->StopButton->setText("Stop\ntimer");

    // Nothing other works
    QRect screenSize = QGuiApplication::screens()[0]->geometry();
    int dialogX = (screenSize.width() / 5) * 3;
    int dialogY = (screenSize.height() / 6) * 3;
    int positionX = ((screenSize.width() - dialogX) / 2);
    int positionY = ((screenSize.height() - dialogY) / 2);
    qDebug() << dialogX << dialogY << positionX << positionY;

    this->move(positionX, positionY);
    this->setFixedSize(QSize(dialogX, dialogY));

    QDir config = QDir(".config/e-1-charger_manager");
    QFile fileTime(config.path() + ("/time"));
    QFile fileConf(config.path() + ("/confirmation"));
    if(config.exists() == false) {
        config.mkpath(config.path());
    }
    else {
        qDebug() << "so the dir exists";
        qDebug() << fileTime;
        fileTime.open(QIODevice::ReadWrite);
        if(fileTime.exists() == true) {
            timeout = fileTime.readAll().replace("\n", "").toInt();
            ui->labelSeconds->setText(QString::number(timeout));
            qDebug() << "Timeout is: " << timeout;
        } else {
            fileTime.resize(0);
            fileTime.write("8"); // default value
        }
        fileTime.close();

        fileConf.open(QIODevice::ReadWrite);
        if(fileConf.exists() == true) {
            bool checked = fileConf.readAll().replace("\n", "").toInt();
            confirmationNeeded = checked;
            ui->confirmationCheckBox->setChecked(checked);
            qDebug() << "Checked is: " << checked;
        } else {
            fileConf.resize(0);
            fileConf.write("1"); // default value
            ui->confirmationCheckBox->setChecked(1);
        }
        fileConf.close();
    }
    ui->progressBar->setRange(0, timeout);
    ui->progressBar->setValue(0);


    timer.singleShot(1000, this, SLOT(secondPassed()));
}

main_menu::~main_menu()
{
    delete ui;
}

void main_menu::secondPassed() {
    if(stop == false) {
        if(timePassed != timeout) {
            timePassed = timePassed + 1;
            ui->progressBar->setValue(timePassed);
            timer.singleShot(1000, this, SLOT(secondPassed()));
        } else {
            if(confirmationNeeded == false) {
                applySettings();
            }
        }
    } else {
        qDebug() << "End timer stop";
    }

}

void main_menu::on_plusButton_clicked()
{
    stop = true;
    timeout = timeout + 1;
    if(timeout >= 11) {
        timeout = 10;
    }
    ui->labelSeconds->setText(QString::number(timeout) + "s");
    QDir config = QDir(".config/e-1-charger_manager");
    QFile file(config.path() + ("/time"));
    file.open(QIODevice::ReadWrite);
    file.resize(0);
    file.write(QString::number(timeout).toUtf8());
    file.close();

}

void main_menu::on_minusButton_clicked()
{
    stop = true;
    timeout = timeout - 1;
    if(timeout <= 2) {
        timeout = 3;
    }
    ui->labelSeconds->setText(QString::number(timeout) + "s");
    QDir config = QDir(".config/e-1-charger_manager");
    QFile file(config.path() + ("/time"));
    file.open(QIODevice::ReadWrite);
    file.resize(0);
    file.write(QString::number(timeout).toUtf8());
    file.close();
}

void main_menu::on_StopButton_clicked()
{
    stop = true;
}

void main_menu::on_cancelButton_clicked()
{
    stop = true;
    exitApp();
}

void main_menu::on_AcceptButton_clicked()
{
    stop = true;
    applySettings();
}

void main_menu::applySettings() {
    qDebug() << "Applying settings";
    stop = true;

    QFile charger("/sys/class/power_supply/mc13892_bat/status");
    charger.open(QIODevice::ReadOnly);
    QString chargerStatus = charger.readAll();
    charger.close();
    if(chargerStatus.contains("Discharging")) {
        QMessageBox::critical(this, tr(""), tr("The device is not charging"));
    } else {
        // i2c set
        QStringList i2cArgs = {"-y", "-f", "3", "0x32", "0xb3", "0x20"};
        launchProcess("/usr/sbin/i2cset", i2cArgs);
        QThread::msleep(200);

        // set host
        // https://chetnachaudhari.github.io/2016-05-08/how-to-enable-debugfs/
        QDir debugDir("/sys/kernel/debug/");
        if (debugDir.entryList(QDir::NoDotAndDotDot | QDir::AllEntries).isEmpty()) {
            // mount -t debugfs none /sys/kernel/debug
            QStringList mount_arg = {"-t", "debugfs", "none", "/sys/kernel/debug"};
            launchProcess("/bin/mount", mount_arg);
            QThread::msleep(100);
        }
        QFile usbDev("/sys/kernel/debug/ci_hdrc.0/role");
        if (usbDev.open(QIODevice::WriteOnly | QIODevice::Text) == false) {
            qDebug() << "Couldn't open usb device file";
        }
        else {
            QTextStream out(&usbDev);
            out << "host" << Qt::endl;
        }
        usbDev.close();

        // One more time to be sure
        launchProcess("/usr/sbin/i2cset", i2cArgs);

        exitApp();
    }
}

void main_menu::exitApp() {
    for(int pid: pidList) {
        unfreezeApp(pid);
    }
    QApplication::exit();
}

void main_menu::on_confirmationCheckBox_stateChanged(int arg1)
{
    QDir config = QDir(".config/e-1-charger_manager");
    QFile fileConf(config.path() + ("/confirmation"));
    fileConf.open(QIODevice::ReadWrite);
    fileConf.resize(0);
    fileConf.write(QString::number(arg1).toUtf8());
    fileConf.close();
    confirmationNeeded = arg1;
}
