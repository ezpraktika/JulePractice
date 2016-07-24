#include "shipitem.h"
#include <QtDebug>
#include <QtMath>
ShipItem::ShipItem()
{

}

QRectF ShipItem::boundingRect() const
{
    float edge = viewLength * qTan(qDegreesToRadians(viewAngle/2));
    return QRectF(-35,-(qMax(5.0f,edge)+5),40+viewLength,qMax(10.0f,2*edge)+10);

}

void ShipItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPolygonF polygon;
    polygon << QPoint(-30,-5) << QPoint(0,-5) << QPoint(5,0) << QPoint(0,5) << QPoint(-30,5);

    painter->setBrush(QBrush(Qt::white));
    painter->drawPolygon(polygon);

    polygon.clear();
    float edge = viewLength * qTan(qDegreesToRadians(viewAngle/2));
    polygon << QPoint(0,0) << QPoint(viewLength,edge) << QPoint(viewLength,-edge);

    painter->setBrush(QBrush(Qt::yellow));
    painter->drawPolygon(polygon);

    painter->setBrush(Qt::NoBrush);
    //painter->drawRect(boundingRect());



    /*
     * if (isViewAvailable){ draw another polygon }
     */

    // Учитывать поворот корабля(мб)

  // painter->drawEllipse(rec.bottomRight().rx()+ viewLength,rec.height() + (rec.topRight().ry() - rec.bottomRight().ry()) / 2 - viewLength * cos(viewAngle/2*3.14/180),10,10);
  //  painter->drawEllipse(rec.bottomRight().rx()+ viewLength,rec.height() + (rec.topRight().ry() - rec.bottomRight().ry()) / 2 + viewLength * cos(viewAngle/2*3.14/180),10,10);
}

void ShipItem::advance(int phase)
{
    // обнулить isNew после первоначальной отрисовки
    if (!phase) return;

    if (isNew){
        qDebug() << QString("%1").arg(startX);
        qDebug() << QString("%1").arg(startY);
        qDebug() << "";
        setPos(startX,startY);
        setRotation(courseAngle);
        isNew = 0;
    }
    else{

        startX += qCos(qDegreesToRadians(courseAngle))*speed;
        qDebug() << QString("%1").arg(startX);

        startY += qSin(qDegreesToRadians(courseAngle))*speed;
        qDebug() << QString("%1").arg(startY);

        setPos(startX,startY);
        setRotation(courseAngle);
    }
}

