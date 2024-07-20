#ifndef MYDATAGRAMM_H
#define MYDATAGRAMM_H
#include <QString>
#include <QHostAddress>

class mydatagramm
{
public:
    mydatagramm();
    ~mydatagramm();
    void setData(int Head,QString Num,QString Data,QHostAddress IpToSend,qint16 PortToSent);
    void addata();
    void addata(QString str);
    void addata(QString str,int l);
    int getHeadlear(){return head;};
    QString getNumberOfMessage(){return num;};
    QString getData(){return data;};
    QHostAddress getIpToSend(){return iptosend;};
    qint16 getPortToSend(){return porttosend;};
    void ressiveData(QByteArray ressivedData);
QByteArray toByteArr();
private:
    int head;
    QString num;
    QByteArray data;
    const QString datasplitter = ":d:";
    QHostAddress iptosend;
    qint16 porttosend;
};

#endif // MYDATAGRAMM_H
