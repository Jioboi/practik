#ifndef SENDTHREAD_H
#define SENDTHREAD_H


#include<QThread>
#include<QTimer>
#include<QFile>
#include <QUdpSocket>
#include"mydatagramm.h"
#include <QQueue>

class sendthread : public QThread
{
Q_OBJECT

public:
    sendthread(QTimer *timer,QUdpSocket *udps,QQueue<QByteArray> *DataBuffer,QQueue<QString> *ipbuffer,QQueue<short>  *portbuffer,QList<QByteArray> *datagrammprevios);
    void setdata(int Type,int length,QString data,QHostAddress addr,qint16 port);
    void run() override;
public slots:
    void TimerOut();
private:
    QUdpSocket *udps;
    QQueue<QByteArray> *DatagrammBuffer;
    QQueue<QString> *IPBuffer;
    QQueue<short> *PortBuffer;
    QTimer *ThreatTimer;
    QUdpSocket *Threatudps;
    QList<QByteArray> *DatagrammPrevios;

};

#endif // SENDTHREAD_H
