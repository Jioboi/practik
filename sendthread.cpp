#include "sendthread.h"

sendthread::sendthread(QTimer *timer,QUdpSocket *udps,QQueue<QByteArray> *DataBuffer,QQueue<QString> *ipbuffer,QQueue<short>  *portbuffer,QList<QByteArray> *datagrammprevios)
{
    Threatudps= udps;
    ThreatTimer=timer;
    DatagrammBuffer=DataBuffer;
    IPBuffer=ipbuffer;
    PortBuffer=portbuffer;
    DatagrammPrevios=datagrammprevios;
}
void sendthread::run(){
    connect(ThreatTimer,SIGNAL(timeout()),this,SLOT(TimerOut()));
}
void slots sendthread::TimerOut(){
        if(!DatagrammBuffer->isEmpty()){
            QByteArray datagramm(DatagrammBuffer->dequeue());
            DatagrammPrevios->append(datagramm);
            QHostAddress ip = QHostAddress(IPBuffer->dequeue());
            qint16 port = PortBuffer->dequeue();
            Threatudps->writeDatagram(datagramm,ip,port);
        }
}
