#ifndef SHIPITEMSTRUCT
#define SHIPITEMSTRUCT

#include <QTime>

struct ShipItemStruct{
    bool turnAlreadyStarted;    //для генерации курса
    qint16 deltaCount;          //
    qreal delta;                //

    quint16 isNew;      //новый корабль или нет
    qreal startX;   //текущие координаты
    qreal startY;   //

    quint16 id;         //индекс
    qreal courseAngle;  //курс
    quint16 speed;      //скорость
    qreal viewAngle;    //угол обзора
    quint16 viewLength; //длина обзора
    QTime timer;        //таймер
    int time;           //время существования (мс)
    qreal pathLength;   //пройденный путь

    ShipItemStruct(){
        turnAlreadyStarted=false;
    }
};

#endif // SHIPITEMSTRUCT

