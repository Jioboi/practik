#include "mydatagramm.h"
#include<QIODevice>
#include <QDataStream>
mydatagramm::mydatagramm()
{
    head='0';
    num="0";
    data="";
}
void mydatagramm::setData(int Head,QString Num,QString Data)
{
    head=Head+'0';
    num=Num;
    data.clear();
    data.append(Data);
}
QByteArray mydatagramm::toByteArr(){

    QByteArray output; ;//1 на заголовок  + 1 на номер пакета + 1 на длину данных + L на данные + 1 на конец строки

    output.append(head);
    if(head=='0'||head=='2'){
        return output;
    }
    output.append(num);
    output.append(datasplitter);
    output.append(data.data());
    output.append(datasplitter);
    //int r=output.size()+sizeof (int);
    //output.append(QString(r));
    return output;
}
void mydatagramm::addata(){
}
void mydatagramm::addata(QString str){
    data.resize(str.size());
    data.clear();
    data.append(str);
}
void mydatagramm::addata(QString str, int l){
    data.resize(l);
    data.clear();
    data.append(str.left(l));
}
void mydatagramm::ressiveData(QByteArray ressivedData){
    if(!ressivedData.isEmpty()){
    head = ressivedData.at(0);
    num = ressivedData.mid(1,ressivedData.indexOf(datasplitter)-1);
    data.clear();
    data = ressivedData.mid(ressivedData.indexOf(datasplitter)+3,ressivedData.indexOf(datasplitter,ressivedData.indexOf(datasplitter)+1)-ressivedData.indexOf(datasplitter)-3);
    }
}
mydatagramm::~mydatagramm(){

}
