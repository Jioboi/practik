#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QNetworkInterface>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    /*QList<QHostAddress> a = QNetworkInterface::allAddresses();
    for(int i=0; i<a.count(); i++)

      {
          if(!a[i].isLoopback())
              if (a[i].protocol() == QAbstractSocket::IPv4Protocol && a[i].isGlobal())
            qDebug() << a[i].toString();

      }*/
    ui->setupUi(this);
    udps= new QUdpSocket();
    mydatagramm = new class mydatagramm();
    mydatagramm->addata();
    udps->bind(QHostAddress::AnyIPv4,10000);
    connect(udps,SIGNAL(readyRead()),SLOT(Read()));
    sendmess(0,0,QHostAddress::Broadcast);
}

MainWindow::~MainWindow()
{
    delete mydatagramm;
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QHostAddress addr = QHostAddress::Broadcast;
    if(ui->checkBox->isCheckable()){
        addr=QHostAddress(ui->lineEdit_5->text());
    }
    if (L==0){
        sendmess(1,3,QHostAddress::Broadcast);
    }
    else{
        sendmess(3,L,QHostAddress::Broadcast);
    }
    ui->textBrowser->append("me:<font color=red> "+ui->lineEdit->text()+"</font>");
    ui->lineEdit->clear();

}
void MainWindow::Read()
{
    QByteArray resiveData;
    QHostAddress sender;
    sender.toIPv4Address();
    quint16 senderPort;
    while (udps->hasPendingDatagrams()){
        resiveData.resize(udps->pendingDatagramSize());
        udps->readDatagram(resiveData.data(),resiveData.size(),&sender,&senderPort);
        QList<QListWidgetItem *> items = ui->listWidget->findItems(sender.toString(),Qt::MatchExactly);
        char h=resiveData.at(0);
        int npack = resiveData.mid(1,resiveData.indexOf(":d:")-1).toInt()-1;
        QString str;
        switch(h){
        case '0':
            if(items.isEmpty()){
                ui->listWidget->addItem(QString(sender.toString()));
            }
            sendmess(2,0,sender);
            break;
        case '1':
            if(ui->listWidget->item(0)->data(0).toString()==sender.toString() && !ui->checkBox->isChecked()) break;
            ui->textBrowser->append(resiveData.mid(resiveData.indexOf(":d:")+3,resiveData.indexOf(":d:",resiveData.indexOf(":d:")+1)-resiveData.indexOf(":d:")-3));
            sendmess(4,0,sender);
            break;
        case '2':
            if(items.isEmpty()){
                ui->listWidget->addItem(sender.toString());
            }
            break;
        case '3':
            if(ui->listWidget->item(0)->data(0).toString()==sender.toString() && !ui->checkBox->isChecked()) break;
            str=str.append(resiveData.mid(resiveData.indexOf(":d:")+3,resiveData.indexOf(":d:",resiveData.indexOf(":d:")+1)-resiveData.indexOf(":d:")-3));
            for (npack;npack>0;npack--) {
                udps->readDatagram(resiveData.data(),resiveData.size(),&sender,&senderPort);
                str=str.append(resiveData.mid(resiveData.indexOf(":d:")+3,resiveData.indexOf(":d:",resiveData.indexOf(":d:")+1)-resiveData.indexOf(":d:")-3));
            }
            ui->textBrowser->append(str);
            sendmess(4,0,sender);
            break;
        case '4':
            ui->textBrowser->append("Ressived");
            break;
        }
        //ui->textBrowser->append("<font color=green>"+QString(resiveData)+"("+sender.toString()+":"+QString("%1").arg(senderPort)+")</font>");
    }
}
void MainWindow::sendmess(int k, int n, QHostAddress addr){
    udps->localAddress();
    QByteArray transmitData;//битовая строка которую отправляем
    int j=0;
    switch (k) {
    case 0: //служебное сообщение для проверки "кто в сети"
        mydatagramm->head='0';
        mydatagramm->num='0';
        udps->writeDatagram(mydatagramm->toByteArr(), addr,10000);
        break;
    case 1: // для отправки данных
        mydatagramm->head='1';
        mydatagramm->num='0';
        mydatagramm->addata(ui->lineEdit->text());
        udps->writeDatagram(mydatagramm->toByteArr(), addr,10000);
        break;
    case 2: //для ответа "в сети"
        mydatagramm->head='2';
        mydatagramm->num='0';
        udps->writeDatagram(mydatagramm->toByteArr(), addr,10000);
        break;
    case 3: //жля сообщений кот. больше чем длинна
        mydatagramm->head='3';
        j=ui->lineEdit->text().size()/n;
        for (int i=0;i<j;i++) {
            mydatagramm->num=j-i+'0';
            mydatagramm->addata(ui->lineEdit->text().mid(i*n,n),n);
            udps->writeDatagram(mydatagramm->toByteArr(), addr,10000);
        }
        break;
     case 4://для квитанции получения
        mydatagramm->head='4';
        mydatagramm->num='0';
        udps->writeDatagram(mydatagramm->toByteArr(), addr,10000);
        break;
    default:
        ui->textBrowser->append("errorMes");
        break;
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    L=ui->lineEdit_4->text().toInt();
}



void MainWindow::on_pushButton_4_clicked()
{
    sendmess(0,0,QHostAddress::Broadcast);
}
