#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>
#include <QString>
#include <iostream>
#include <QNetworkInterface>
#include <QFileSystemModel>
#include <QQueue>

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
    void Read();//чтение данных
    void TimrOut();//истечение таймера
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
    Ui::MainWindow *ui;
    QHostAddress address;
    QUdpSocket *udps;// указатель на сокет
    mydatagramm *mydatagramm; //  датаграмма
    QQueue<QByteArray> DatagrammBuffer;
    QQueue<QString> IPBuffer;
    QQueue<short> PortBuffer;
    int L=0;
    QString str;
    enum Headler{
        ScanMessage,
        DataMessage,
        AnsverMessage,
        LimitedDataMessage,
        ReceiptMessage,
        FileMessage,
        sendFile,
    };
    qint16 myport;
    qint16 AMport;
    QTimer *timer;
    QFile transmitefile;
};

#endif // MAINWINDOW_H
