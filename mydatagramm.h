#ifndef MYDATAGRAMM_H
#define MYDATAGRAMM_H
#include <QString>
#include <QHostAddress>

class mydatagramm
{
public:
    mydatagramm();
    ~mydatagramm();
    void setData(int Head,QString Num,QString Data);
    void addata();
    void addata(QString str);
    void addata(QString str,int l);
    int getHeadlear(){return head;};
    QString getNumberOfMessage(){return num;};
    QString getData(){return data;};
    void ressiveData(QByteArray ressivedData);
QByteArray toByteArr();
private:
    int head;
    QString num;
    QByteArray data;
    const QString datasplitter = ":d:";
};

#endif // MYDATAGRAMM_H
