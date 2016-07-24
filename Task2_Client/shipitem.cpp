#include "shipitem.h"
#include <QtDebug>
#include <QtMath>
ShipItem::ShipItem()
{
    shipSize = 1;
    isViewVisible = true;
}

QRectF ShipItem::boundingRect() const
{
    float edge = viewLength * qTan(qDegreesToRadians(viewAngle/2));
    return QRectF(-35,-(qMax(5.0f,edge)+5),40+viewLength,qMax(10.0f,2*edge)+10); // -35 -> -50

}

void ShipItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPolygonF polygon;
    polygon << QPoint(-30,-5) << QPoint(0,-5) << QPoint(5,0) << QPoint(0,5) << QPoint(-30,5);

    painter->setBrush(QBrush(Qt::white));
    painter->drawPolygon(polygon);

    polygon.clear();
    if(isViewVisible){
        float edge = viewLength * qTan(qDegreesToRadians(viewAngle/2));
        polygon << QPoint(0,0) << QPoint(viewLength,edge) << QPoint(viewLength,-edge);

        painter->setBrush(QBrush(Qt::yellow));
        painter->drawPolygon(polygon);
    }


    painter->setBrush(Qt::NoBrush);


//    painter->drawRect(boundingRect());



    /*
     * if (isViewAvailable){ draw another polygon }
     */

}

void ShipItem::advance(int phase)
{
    // обнулить isNew после первоначальной отрисовки
    if (!phase) return;

    if (isNew){
        isNew = 0;
    }
    else{
        prevX = startX;
        prevY = startY;
        startX += qCos(qDegreesToRadians(courseAngle))*speed;
        startY += qSin(qDegreesToRadians(courseAngle))*speed;
    }

    setPos(startX,startY);
    setRotation(courseAngle);
    setScale(shipSize); // привязать к ползунку(масштаб)

}

