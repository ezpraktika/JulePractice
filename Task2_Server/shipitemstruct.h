#ifndef SHIPITEMSTRUCT
#define SHIPITEMSTRUCT

#include <QTime>

struct ShipItemStruct{

    quint16 isNew;
    qreal startX;
    qreal startY;

    quint16 id;
    qreal courceAngle;
    quint16 speed;
    qreal viewAngle;
    quint16 viewLength;
    QTime time;
    qreal pathLength;
};

#endif // SHIPITEMSTRUCT

