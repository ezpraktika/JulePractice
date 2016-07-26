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

    quint16 id;         //индекс
    qreal courseAngle;  //курс
    quint16 speed;      //скорость
    qreal viewAngle;    //угол обзора
    quint16 viewLength; //длина обзора
    QTime time;         //время существования use start and elapsed
    qreal pathLength;   //пройденный путь


    quint16 shipSize;   //размер корабля
    bool isViewVisible; //вкл/выкл область видимости

    quint16 isNew;  //новый корабль или нет
    qreal startX;   //координаты на текущем этапе
    qreal startY;   //

    qreal prevX;    //координаты на предыдущем этапе
    qreal prevY;    //

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    void advance(int phase);


};

#endif // SHIPITEM_H
