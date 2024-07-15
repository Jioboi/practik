#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QNetworkInterface>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    udps= new QUdpSocket();
    mydatagramm.addata();
    myport=10000;
    AMport=10000;
    udps->bind(QHostAddress::AnyIPv4,myport);
    connect(udps,SIGNAL(readyRead()),SLOT(Read()));
    sendmess(ScanMessage,0,QHostAddress::Broadcast,AMport);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_sendMButton_clicked()
{
    QHostAddress addr;
    qint16 port=AMport;
    if(ui->isManualCheckBox->isChecked()){
        addr=QHostAddress(ui->LineEditManualSet_IpPort->text().left(ui->LineEditManualSet_IpPort->text().indexOf(":")));
        port=ui->LineEditManualSet_IpPort->text().right(ui->LineEditManualSet_IpPort->text().size()-ui->LineEditManualSet_IpPort->text().indexOf(":")-1).toShort();
        if (L==0){
            sendmess(DataMessage,3,addr,port);
        }
        else{
            sendmess(LimitedDataMessage,L,addr,port);
        }
    }
    else{
        for (int i=1;i<ui->IPinWebListWidget->count();i++) {
            addr=QHostAddress(ui->IPinWebListWidget->item(i)->data(0).toString());
            if (L==0){
                sendmess(DataMessage,3,addr,port);
            }
            else{
                sendmess(LimitedDataMessage,L,addr,port);
            }
        }
    }
    ui->chatTextBrowser->append("me:<font color=red> "+ui->MessageLineEdit->text()+"</font>");
    ui->MessageLineEdit->clear();

}
void MainWindow::Read()
{
    QByteArray ressivedData;
    QHostAddress sender;
    sender.toIPv4Address();
    quint16 senderPort;
    while (udps->hasPendingDatagrams()){

        ressivedData.resize(udps->pendingDatagramSize());
        udps->readDatagram(ressivedData.data(),ressivedData.size(),&sender,&senderPort);
        QList<QListWidgetItem *> items = ui->IPinWebListWidget->findItems(sender.toString()+":"+QString("%1").arg(senderPort),Qt::MatchExactly);
        mydatagramm.ressiveData(ressivedData);
        int h=mydatagramm.getHeadlear()-'0';
        int npack = mydatagramm.getNumberOfMessage().toInt()-1;
        QString str;
        switch(h){
        case ScanMessage:
            if(items.isEmpty()){
                items = ui->IPinWebListWidget->findItems(sender.toString(),Qt::MatchContains);
                if(!items.isEmpty()){
                    items.first()->setText(sender.toString()+":"+QString("%1").arg(senderPort));
                }
                else{
                ui->IPinWebListWidget->addItem(sender.toString()+":"+QString("%1").arg(senderPort));
                }
            }
            sendmess(AnsverMessage,0,QHostAddress::Broadcast,10000);
            break;
        case DataMessage:
            if(ui->IPinWebListWidget->item(0)->data(0).toString()==sender.toString() && !ui->isManualCheckBox->isChecked()) break;
            ui->chatTextBrowser->append(mydatagramm.getData());
            sendmess(ReceiptMessage,0,sender,senderPort);
            break;
        case AnsverMessage:
            if(items.isEmpty()){
                ui->IPinWebListWidget->addItem(sender.toString()+":"+QString("%1").arg(senderPort));
            }
            break;
        case LimitedDataMessage:
            if(ui->IPinWebListWidget->item(0)->data(0).toString()==sender.toString() && !ui->isManualCheckBox->isChecked()) break;
            str=str.append(mydatagramm.getData());
            for (npack;npack>0;npack--) {
                udps->readDatagram(ressivedData.data(),ressivedData.size(),&sender,&senderPort);
                mydatagramm.ressiveData(ressivedData);
                str=str.append(mydatagramm.getData());
            }
            ui->chatTextBrowser->append(str);
            sendmess(ReceiptMessage,0,sender,senderPort);
            break;
        case ReceiptMessage:
            ui->chatTextBrowser->append("Ressived");
            break;
        }
    }
}
void MainWindow::sendmess(int k, int n, QHostAddress addr,quint16 port){
    int j=0;
    switch (k) {
    case ScanMessage: //служебное сообщение для проверки "кто в сети"
        mydatagramm.setData(ScanMessage,QString(j),"");
        udps->writeDatagram(mydatagramm.toByteArr(), addr,port);
        break;
    case DataMessage: // для отправки данных

        mydatagramm.setData(DataMessage,QString(j),ui->MessageLineEdit->text());
        udps->writeDatagram(mydatagramm.toByteArr(), addr,port);
        break;
    case AnsverMessage: //для ответа "в сети"
        mydatagramm.setData(AnsverMessage,QString(j),ui->MessageLineEdit->text());
        udps->writeDatagram(mydatagramm.toByteArr(), addr,port);
        break;
    case LimitedDataMessage: //жля сообщений кот. больше чем введенная длинна

        j=ui->MessageLineEdit->text().size()/n;
        for (int i=0;i<j;i++) {
            mydatagramm.setData(LimitedDataMessage,QString("%1").arg(j-i),ui->MessageLineEdit->text().mid(i*n,n));
            udps->writeDatagram(mydatagramm.toByteArr(), addr,port);
        }
        break;
     case ReceiptMessage://для квитанции получения
        mydatagramm.setData(ReceiptMessage,QString(j),"");
        udps->writeDatagram(mydatagramm.toByteArr(), addr,port);
        break;
    default:
        ui->chatTextBrowser->append("errorMes");
        break;
    }
}

void MainWindow::on_setNumberOfBytsButton_clicked()
{
    L=ui->NumberOfBytesLineEdit->text().toInt();
}



void MainWindow::on_scanButton_clicked()
{
    sendmess(ScanMessage,0,QHostAddress::Broadcast,myport);
}

void MainWindow::on_ManualSetButton_myport_clicked()
{
    if(myport=ui->LineEditManualSet_myport->text().toInt()<64000){
    myport=ui->LineEditManualSet_myport->text().toShort();
    udps->close();
    udps->bind(QHostAddress::AnyIPv4,myport);
    connect(udps,SIGNAL(readyRead()),SLOT(Read()));
    sendmess(ScanMessage,0,QHostAddress::Broadcast,myport);
    }
    else {ui->LineEditManualSet_myport->text()="PortErr";}
}

void MainWindow::on_ManualSetButton_port_clicked()
{
    AMport=ui->LineEditManualSet_port->text().toShort();
}
