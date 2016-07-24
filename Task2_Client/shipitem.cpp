#include "shipitem.h"

ShipItem::ShipItem()
{

}

QRectF ShipItem::boundingRect() const
{
    return QRectF(0,0,20,20);
}

void ShipItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

}

void ShipItem::advance(int phase)
{

}

