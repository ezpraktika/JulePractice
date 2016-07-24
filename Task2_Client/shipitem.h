#ifndef SHIPITEM_H
#define SHIPITEM_H

#include <QPainter>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QTime>

class ShipItem : public QGraphicsItem
{
public:
    ShipItem();
    quint16 id;
    qreal courceAngle;
    quint16 speed;
    qreal viewAngle;
    quint16 viewLength;
    QTime time; //use start and elapsed
    qreal pathLength;

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    void advance(int phase);

//private:

};

#endif // SHIPITEM_H
