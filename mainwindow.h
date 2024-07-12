#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>
#include <QString>
#include "mydatagramm.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void Read();

private slots:
    void on_pushButton_clicked();// здесь будет оправка данных по нажатию на кнопку
    void sendmess(int k, int lehgth, QHostAddress addr);

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::MainWindow *ui;
    QHostAddress address;
    QUdpSocket *udps;// указатель на сокет
    mydatagramm *mydatagramm; //  указатель на
    int L=0;
};

#endif // MAINWINDOW_H
