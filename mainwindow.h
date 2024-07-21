#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>
#include <QString>
#include <iostream>
#include <QNetworkInterface>
#include <QFileSystemModel>
#include <QQueue>
#include <QTimer>
#include<QSql>
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QThread>
#include "mydatagramm.h"
#include"sendthread.h"

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
    void Read();//чтение данных
private slots:
    void on_sendMButton_clicked();// здесь будет оправка данных по нажатию на кнопку
    void sendmess(int k, int lehgth, QHostAddress addr,quint16 port);//фукия отправки данных

    void on_setNumberOfBytsButton_clicked();//задание определенного количества байт на сообщение

    void on_scanButton_clicked();//сканирование клиентов в сети


    void on_ManualSetButton_myport_clicked();

    void on_ManualSetButton_port_clicked();

    void on_sendFileButton_clicked();

    void on_setFreqencyButton_clicked();

    void on_treeView_doubleClicked(const QModelIndex &index);

private:
    sendthread *sendthread;
    Ui::MainWindow *ui;
    QHostAddress address;
    QUdpSocket *udps;
    mydatagramm *mydatagramm; //  датаграмма
    QQueue<QByteArray> DatagrammBuffer;
    QList<QByteArray> DatagrammPrevios;
    QList<int> countNumMess;
    QQueue<QString> IPBuffer;
    QQueue<short> PortBuffer;
    QSqlDatabase ChatDB;
    int L=0;
    QString str;
    QSqlQuery chatquery;
    enum Headler{
        ScanMessage,
        DataMessage,
        AnsverMessage,
        LimitedDataMessage,
        ReceiptMessage,
        FileMessage,
        sendFile,
        ReqvestMess,
        ReqvestMessAnsv
    };
    qint16 myport;
    qint16 AMport;
    QTimer *timer;
    QFile transmitefile;
};

#endif // MAINWINDOW_H
