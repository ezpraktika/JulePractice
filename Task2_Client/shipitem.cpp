#include "shipitem.h"
#include <QtDebug>
ShipItem::ShipItem()
{

}

QRectF ShipItem::boundingRect() const
{
    return QRectF(0,0,150,150);
    QRectF rec = QRectF(0,50,40,10);
    QBrush brush(Qt::gray);
}

void ShipItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

    painter->fillRect(rec,brush);
    painter->drawRect(rec);
    // Учитывать поворот корабля(мб)
    painter->drawEllipse(rec.bottomRight().rx()+ viewLength,rec.height() + (rec.topRight().ry() - rec.bottomRight().ry()) / 2 - viewLength * cos(viewAngle/2*3.14/180),10,10);
    painter->drawEllipse(rec.bottomRight().rx()+ viewLength,rec.height() + (rec.topRight().ry() - rec.bottomRight().ry()) / 2 + viewLength * cos(viewAngle/2*3.14/180),10,10);
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
        isNew = 0;
    }
    else{

        startX += cos(courceAngle*3.14/180)*speed;
        qDebug() << QString("%1").arg(startX);

        startY += sin(courceAngle*3.14/180)*speed;
        qDebug() << QString("%1").arg(startY);

        qDebug() << "";
        setPos(startX,startY);
        setRotation(courceAngle);
    }
}

