#ifndef SHIPITEM_H
#define SHIPITEM_H

#include <QPainter>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QTime>
#include <QtDebug>
#include <QtMath>

class ShipItem : public QGraphicsItem
{
public:
    ShipItem();
    quint16 id;
    qreal courseAngle;
    quint16 speed;
    qreal viewAngle;
    quint16 viewLength;
    QTime time; //use start and elapsed
    qreal pathLength;


    quint16 shipSize;
    bool isViewVisible;


    quint16 isNew;
    qreal startX;
    qreal startY;

    qreal prevX;
    qreal prevY;

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    void advance(int phase);

//private:

};

#endif // SHIPITEM_H
