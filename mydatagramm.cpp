#include "mydatagramm.h"
#include<QIODevice>
#include <QDataStream>

mydatagramm::mydatagramm()
{

}
QByteArray mydatagramm::toByteArr(){

    QByteArray output; ;//1 на заголовок  + 1 на номер пакета + 1 на длину данных + L на данные + 1 на конец строки

    output.append(head);
    if(head=='0'||head=='2'){
        return output;
    }
    //output.append('\0');
    output.append(num);
    output.append(":d:");
    output.append(data->data());
    output.append(":d:");
    qint32 r=output.size()+sizeof (qint64);
    QString myString;
    myString.resize(sizeof(r));
    myString = QString::number(r,10).toUpper();
    output.append(myString);
    //QDataStream out(&output,QIODevice::WriteOnly);
    return output;
}
void mydatagramm::addata(){
    data = new QByteArray();
}
void mydatagramm::addata(QString str){
    data->resize(str.size());
    data->clear();
    data->append(str);
}
void mydatagramm::addata(QString str, int l){
    data->resize(l);
    data->clear();
    data->append(str.left(l));
}
mydatagramm::~mydatagramm(){
    delete data;
}
