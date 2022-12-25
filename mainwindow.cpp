#include "mainwindow.h"
#include "main_menu.h"
#include "global.h"

#include "ui_mainwindow.h"

#include <QTimer>
#include <QScreen>
#include <QDebug>
#include <QDir>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // remove this after
    this->ui->centralwidget->setStyleSheet("background-image:url(\"/external_root/tmp/background.png\"); background-position: center;");

    QTimer::singleShot(100, this, &MainWindow::launchDialog);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::launchDialog() {
    qDebug() << "Launching dialog";

    main_menu* mainMenuDialog = new main_menu;
    mainMenuDialog->setWindowFlag(Qt::FramelessWindowHint);
    mainMenuDialog->exec();
}
