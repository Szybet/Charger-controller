#ifndef LOCKSCREEN_H
#define LOCKSCREEN_H

#include <QDialog>
#include <QFile>
#include <QTextStream>
#include <fstream>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class main_menu; }
QT_END_NAMESPACE

using namespace std;

class main_menu : public QDialog
{
    Q_OBJECT

public:
    main_menu(QDialog *parent = nullptr);
    ~main_menu();
    int timeout = 4;
    int timePassed = 0;
    QTimer timer;
    bool stop = false;

public slots:
    void secondPassed();
    void applySettings();


private slots:


    void on_plusButton_clicked();

    void on_minusButton_clicked();

    void on_StopButton_clicked();

    void on_cancelButton_clicked();

    void on_AcceptButton_clicked();

private:
    Ui::main_menu *ui;

};
#endif // LOCKSCREEN_H
