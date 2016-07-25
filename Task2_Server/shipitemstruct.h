#ifndef SHIPITEMSTRUCT
#define SHIPITEMSTRUCT

#include <QTime>

struct ShipItemStruct{
    bool turnAlreadyStarted;
    qint16 deltaCount;
    qreal delta;

    quint16 isNew;
    qreal startX;
    qreal startY;

    quint16 id;
    qreal courseAngle;
    quint16 speed;
    qreal viewAngle;
    quint16 viewLength;
    QTime time;
    qreal pathLength;

    ShipItemStruct(){
        turnAlreadyStarted=false;
    }
};

#endif // SHIPITEMSTRUCT

