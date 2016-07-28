#include "shipitem.h"

ShipItem::ShipItem()
{
    shipSize = 0;
    isViewVisible = true;
}

QRectF ShipItem::boundingRect() const
{
    float edge = viewLength * qTan(qDegreesToRadians(viewAngle/2));
    return QRectF(-35,-(qMax(5.0f,edge)+5),50+viewLength,qMax(10.0f,2*edge)+10); // -35 -> -50, 50 -> 65

}

void ShipItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPolygonF polygon;
    polygon << QPoint(-20,-5) << QPoint(10,-5) << QPoint(15,0) << QPoint(10,5) << QPoint(-20,5);

    painter->setBrush(QBrush(Qt::white));
    painter->drawPolygon(polygon);

    painter->drawText(-5,-15,QString::number(id+1));

    polygon.clear();
    if(isViewVisible){
        float edge = viewLength * qTan(qDegreesToRadians(viewAngle/2));
        polygon << QPoint(10,0) << QPoint(viewLength+10,edge) << QPoint(viewLength+10,-edge);

        painter->setBrush(QBrush(Qt::yellow));
        painter->drawPolygon(polygon);
    }


    painter->setBrush(Qt::NoBrush);


    //painter->drawRect(boundingRect());

}

void ShipItem::advance(int phase)
{
    // обнулить isNew после первоначальной отрисовки
    if (!phase) return;

    if (isNew){
        isNew = 0;
        points.append(QPointF(startX,startY));
    }
    else{
        startX += qCos(qDegreesToRadians(courseAngle))*speed;
        startY += qSin(qDegreesToRadians(courseAngle))*speed;
        points.append(QPointF(startX,startY));
    }

    setPos(startX,startY);      //переместить по новым координатам
    setRotation(courseAngle);   //повернуть в соответствии с курсом
    setScale(1+shipSize*0.2);         //увеличить размеры

}

