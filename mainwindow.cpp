#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    udps= new QUdpSocket();
    mydatagramm = new class mydatagramm;
    myport=10000;
    AMport=10000;
    QFileSystemModel* model =  new QFileSystemModel;
    model->setRootPath(QDir::currentPath());
    ui->treeView->setModel(model);
    udps->bind(QHostAddress::AnyIPv4,myport);
    connect(udps,SIGNAL(readyRead()),SLOT(Read()));
    timer= new QTimer;
    connect(timer,SIGNAL(timeout()),SLOT(TimrOut()));
    timer->start(10);
    sendmess(ScanMessage,0,QHostAddress("192.168.1.255"),AMport);
    ChatDB = QSqlDatabase::addDatabase("QPSQL");
    ChatDB.setDatabaseName("ChatDB");
    ChatDB.setUserName("ChatUser");
    ChatDB.setPassword("12345");
    ChatDB.open();
    chatquery.prepare("INSERT INTO ChatDB(transmitterIP_Port,ressiverIP_Port,TypeOfMessage,DATA)" "VALUES (:transmitterIP_Port,:ressiverIP_Port,:TypeOfMessage,:DATA)");
    sendthread = new class sendthread(timer,udps,&DatagrammBuffer,&IPBuffer,&PortBuffer,&DatagrammPrevios);
    sendthread->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_sendMButton_clicked()
{
    QHostAddress addr;
    qint16 port=AMport;
    QString str;
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
            addr=QHostAddress(ui->IPinWebListWidget->item(i)->data(0).toString().left(ui->IPinWebListWidget->item(i)->data(0).toString().indexOf(":")));
            port=ui->IPinWebListWidget->item(i)->data(0).toString().right(ui->IPinWebListWidget->item(i)->data(0).toString().size() - ui->IPinWebListWidget->item(i)->data(0).toString().indexOf(":")-1).toShort();
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
    QFile RessivesdFile;
    QTextStream out;
    while (udps->hasPendingDatagrams()){

        ressivedData.resize(udps->pendingDatagramSize());
        udps->readDatagram(ressivedData.data(),ressivedData.size(),&sender,&senderPort);
        QList<QListWidgetItem *> items = ui->IPinWebListWidget->findItems(sender.toString()+":"+QString("%1").arg(senderPort),Qt::MatchExactly);
        mydatagramm->ressiveData(ressivedData);
        int h=mydatagramm->getHeadlear();
        int npack = mydatagramm->getNumberOfMessage().toInt();
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
            sendmess(AnsverMessage,0,sender,senderPort);
            break;
        case DataMessage:
            if(ui->IPinWebListWidget->item(0)->data(0).toString()==sender.toString() && !ui->isManualCheckBox->isChecked()) break;
            ui->chatTextBrowser->append(mydatagramm->getData());
            sendmess(ReceiptMessage,0,sender,senderPort);
            chatquery.bindValue(":transmitterIP_Port",ui->IPinWebListWidget->item(0)->data(0).toString());
            chatquery.bindValue(":ressiverIP_Port",sender.toString()+":"+QString("%1").arg(senderPort));
            chatquery.bindValue(":TypeOfMessage",mydatagramm->getHeadlear());
            chatquery.bindValue(":DATA",mydatagramm->getData());
            chatquery.exec();
            break;
        case AnsverMessage:
            if(items.isEmpty()){
                ui->IPinWebListWidget->addItem(sender.toString()+":"+QString("%1").arg(senderPort));
            }
            break;
        case LimitedDataMessage:
            if(ui->IPinWebListWidget->item(0)->data(0).toString()==sender.toString() && !ui->isManualCheckBox->isChecked()) break;
            str=str.append(mydatagramm->getData());
            countNumMess.append(npack);
            if(npack==0){
                int i = countNumMess.takeFirst();
                while (!countNumMess.isEmpty()) {
                    int j = countNumMess.takeFirst();
                    if(i!=j+1){
                        sendmess(ReqvestMess,i-1,sender,senderPort);
                        break;
                    }
                    i=j;
                }
                ui->chatTextBrowser->append(str);
                sendmess(ReceiptMessage,0,sender,senderPort);
                chatquery.bindValue(":transmitterIP_Port",ui->IPinWebListWidget->item(0)->data(0).toString());
                chatquery.bindValue(":ressiverIP_Port",sender.toString()+":"+QString("%1").arg(senderPort));
                chatquery.bindValue(":TypeOfMessage",mydatagramm->getHeadlear());
                chatquery.bindValue(":DATA",mydatagramm->getData());
                chatquery.exec();
                str.clear();
            }
            break;
        case ReceiptMessage:
            ui->chatTextBrowser->append("Ressived");
            break;
        case FileMessage:
            RessivesdFile.setFileName("ressived.txt");
            RessivesdFile.open(QIODevice::Append);
            out.setDevice(&RessivesdFile);
            if(mydatagramm->getData()=="EndOfFile"){
                int i = countNumMess.takeFirst();
                while (!countNumMess.isEmpty()) {
                    int j = countNumMess.takeFirst();
                    if(i!=j-1){
                        sendmess(ReqvestMess,i+1,sender,senderPort);
                        break;
                    }
                    i=j;
                }
                RessivesdFile.close();
                break;
            }
            out<<mydatagramm->getData();
            break;
        case ReqvestMess:
            sendmess(ReqvestMessAnsv,npack,sender,senderPort);
            break;
        case ReqvestMessAnsv:
            str.insert(npack,mydatagramm->getData());
        }
    }
}
void MainWindow::sendmess(int k, int n, QHostAddress addr,quint16 port)
{
    int j=0;
    QTextStream transmit;
    QFile file;
    switch (k) {
    case ScanMessage: //служебное сообщение для проверки "кто в сети"
        mydatagramm->setData(ScanMessage,QString(j),"");
        DatagrammBuffer.enqueue(mydatagramm->toByteArr());
        IPBuffer.enqueue(addr.toString());
        PortBuffer.enqueue(port);
        break;
    case DataMessage: // для отправки данных
        mydatagramm->setData(DataMessage,QString(j),ui->MessageLineEdit->text());
        chatquery.bindValue(":transmitterIP_Port",ui->IPinWebListWidget->item(0)->data(0).toString());
        chatquery.bindValue(":ressiverIP_Port",addr.toString()+":"+QString("%1").arg(port));
        chatquery.bindValue(":TypeOfMessage",mydatagramm->getHeadlear());
        chatquery.bindValue(":DATA",mydatagramm->getData());
        chatquery.exec();
        DatagrammBuffer.enqueue(mydatagramm->toByteArr());
        IPBuffer.enqueue(addr.toString());
        PortBuffer.enqueue(port);
        break;
    case AnsverMessage: //для ответа "в сети"
        mydatagramm->setData(AnsverMessage,QString(j),ui->MessageLineEdit->text());
        DatagrammBuffer.enqueue(mydatagramm->toByteArr());
        IPBuffer.enqueue(addr.toString());
        PortBuffer.enqueue(port);
        break;
    case LimitedDataMessage: //жля сообщений кот. больше чем введенная длинна
        chatquery.bindValue(":transmitterIP_Port",ui->IPinWebListWidget->item(0)->data(0).toString());
        chatquery.bindValue(":ressiverIP_Port",addr.toString()+":"+QString("%1").arg(port));
        chatquery.bindValue(":TypeOfMessage",mydatagramm->getHeadlear());
        chatquery.bindValue(":DATA",ui->MessageLineEdit->text());
        chatquery.exec();
        j=ui->MessageLineEdit->text().size()/n;
        if(ui->MessageLineEdit->text().size()%n!=0){
            j++;
        }
        for (int i=0;i<j;i++) {
            mydatagramm->setData(LimitedDataMessage,QString("%1").arg(j-i-1),ui->MessageLineEdit->text().mid(i*n,n));
            DatagrammBuffer.enqueue(mydatagramm->toByteArr());
            IPBuffer.enqueue(addr.toString());
            PortBuffer.enqueue(port);
        }

        break;
     case ReceiptMessage://для квитанции получения
        mydatagramm->setData(ReceiptMessage,QString(j),"");
        DatagrammBuffer.enqueue(mydatagramm->toByteArr());
        IPBuffer.enqueue(addr.toString());
        PortBuffer.enqueue(port);
        break;
    case FileMessage://для передачи файлов
        if (n==0){n=8100;}
        file.setFileName(ui->FilePathLineEdit->text());
        j=file.size()/n;
        file.open(QIODevice::ReadOnly);
        transmit.setDevice(&file);
        while(!file.atEnd()){;
            mydatagramm->setData(FileMessage,QString(j),transmit.read(n));
            DatagrammBuffer.enqueue(mydatagramm->toByteArr());
            IPBuffer.enqueue(addr.toString());
            PortBuffer.enqueue(port);
        }
        mydatagramm->setData(FileMessage,QString(j+1),"EndOfFile");
        break;
    case ReqvestMess:
        mydatagramm->setData(ReqvestMess,QString(n),"");
        DatagrammBuffer.enqueue(mydatagramm->toByteArr());
        IPBuffer.enqueue(addr.toString());
        PortBuffer.enqueue(port);
    case ReqvestMessAnsv:
        mydatagramm->setData(ReqvestMessAnsv,QString(n), QString(DatagrammPrevios.takeAt(n)));
        DatagrammBuffer.enqueue(mydatagramm->toByteArr());
        IPBuffer.enqueue(addr.toString());
        PortBuffer.enqueue(port);
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
    sendmess(ScanMessage,0,QHostAddress("192.168.1.255"),myport);
}

void MainWindow::on_ManualSetButton_myport_clicked()
{
    myport=ui->LineEditManualSet_myport->text().toShort();
    udps->close();
    udps->bind(QHostAddress::AnyIPv4,myport);
    connect(udps,SIGNAL(readyRead()),SLOT(Read()));
    sendmess(ScanMessage,0,QHostAddress("192.168.1.255"),myport);
}

void MainWindow::on_ManualSetButton_port_clicked()
{
    AMport=ui->LineEditManualSet_port->text().toShort();
}


void MainWindow::on_sendFileButton_clicked()
{
    QHostAddress addr;
    qint16 port=AMport;
    if(ui->isManualCheckBox->isChecked()){
        addr=QHostAddress(ui->LineEditManualSet_IpPort->text().left(ui->LineEditManualSet_IpPort->text().indexOf(":")));
        port=ui->LineEditManualSet_IpPort->text().right(ui->LineEditManualSet_IpPort->text().size()-ui->LineEditManualSet_IpPort->text().indexOf(":")-1).toShort();
        if (L==0){
            sendmess(FileMessage,0,addr,port);
        }
        else{
            sendmess(FileMessage,L,addr,port);
        }
    }
    else{
        for (int i=1;i<ui->IPinWebListWidget->count();i++) {
            addr=QHostAddress(ui->IPinWebListWidget->item(i)->data(0).toString().left(ui->IPinWebListWidget->item(i)->data(0).toString().indexOf(":")));
            port=ui->IPinWebListWidget->item(i)->data(0).toString().right(ui->IPinWebListWidget->item(i)->data(0).toString().size() - ui->IPinWebListWidget->item(i)->data(0).toString().indexOf(":")-1).toShort();
            if (L==0){
                sendmess(FileMessage,0,addr,port);
            }
            else{
                sendmess(FileMessage,L,addr,port);
            }
        }
    }
}

void MainWindow::on_setFreqencyButton_clicked()
{
    float i = ui->FriqencySetLineEdit->text().toFloat();
    timer->setInterval(1/i);
}

void MainWindow::on_treeView_doubleClicked(const QModelIndex &index)
{
    QFileSystemModel *model = new QFileSystemModel;
    QString str= model->filePath(index);
    ui->FilePathLineEdit->text().clear();
    ui->FilePathLineEdit->setText(str);
}
