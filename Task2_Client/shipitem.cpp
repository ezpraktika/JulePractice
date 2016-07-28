#include "shipitem.h"

ShipItem::ShipItem()
{
    shipSize = 0;
    isViewVisible = true;
}

ShipItem::~ShipItem()
{

}

QRectF ShipItem::boundingRect() const
{
    float edge = viewLength * qTan(qDegreesToRadians(viewAngle/2));
    return QRectF(-35,-(qMax(5.0f,edge)+5),50+viewLength,qMax(10.0f,2*edge)+10); // -35 -> -50, 50 -> 65

}

void ShipItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //корабль
    QPolygonF polygon;
    polygon << QPoint(-20,-5) << QPoint(10,-5) << QPoint(15,0) << QPoint(10,5) << QPoint(-20,5);

    painter->setBrush(QBrush(Qt::white));
    painter->drawPolygon(polygon);

    painter->drawText(-5,-15,QString::number(id+1));

    polygon.clear();

    //область видимости
    if(isViewVisible){
        float edge = viewLength * qTan(qDegreesToRadians(viewAngle/2));
        polygon << QPoint(10,0) << QPoint(viewLength+10,edge) << QPoint(viewLength+10,-edge);

        painter->setBrush(QBrush(Qt::yellow));
        painter->drawPolygon(polygon);
    }

    painter->setBrush(Qt::NoBrush);
}

void ShipItem::advance(int phase)
{

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
    setScale(1+shipSize*0.2);   //увеличить размеры

}



qreal ShipItem::getStartY()
{
    return startY;
}

void ShipItem::setStartY(const qreal &value)
{
    startY = value;
}

qreal ShipItem::getStartX()
{
    return startX;
}

void ShipItem::setStartX(const qreal &value)
{
    startX = value;
}

quint16 ShipItem::getIsNew()
{
    return isNew;
}

void ShipItem::setIsNew(const quint16 &value)
{
    isNew = value;
}

bool ShipItem::getIsViewVisible()
{
    return isViewVisible;
}

void ShipItem::setIsViewVisible(bool value)
{
    isViewVisible = value;
}

quint16 ShipItem::getShipSize()
{
    return shipSize;
}

void ShipItem::setShipSize(const quint16 &value)
{
    shipSize = value;
}

qreal ShipItem::getPathLength()
{
    return pathLength;
}

void ShipItem::setPathLength(const qreal &value)
{
    pathLength = value;
}

int ShipItem::getTime()
{
    return time;
}

void ShipItem::setTime(int value)
{
    time = value;
}

quint16 ShipItem::getViewLength()
{
    return viewLength;
}

void ShipItem::setViewLength(const quint16 &value)
{
    viewLength = value;
}

qreal ShipItem::getViewAngle()
{
    return viewAngle;
}

void ShipItem::setViewAngle(const qreal &value)
{
    viewAngle = value;
}

quint16 ShipItem::getSpeed()
{
    return speed;
}

void ShipItem::setSpeed(const quint16 &value)
{
    speed = value;
}

qreal ShipItem::getCourseAngle()
{
    return courseAngle;
}

void ShipItem::setCourseAngle(const qreal &value)
{
    courseAngle = value;
}

quint16 ShipItem::getId()
{
    return id;
}

void ShipItem::setId(const quint16 &value)
{
    id = value;
}

