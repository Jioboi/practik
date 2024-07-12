#ifndef MYDATAGRAMM_H
#define MYDATAGRAMM_H
#include <QString>

class mydatagramm
{
public:
    mydatagramm();
    ~mydatagramm();
    void addata();
    void addata(QString str);
    void addata(QString str,int l);
QByteArray toByteArr();
    char head;
    char num;
    QByteArray *data;
};

#endif // MYDATAGRAMM_H
