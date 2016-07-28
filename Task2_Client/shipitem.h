#ifndef SHIPITEM_H
#define SHIPITEM_H

#include <QPainter>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QtDebug>
#include <QtMath>
#include <QTime>

class ShipItem : public QGraphicsItem
{
public:
    ShipItem();
    ~ShipItem();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    quint16 getId() ;
    void setId(const quint16 &value);

    qreal getCourseAngle() ;
    void setCourseAngle(const qreal &value);

    quint16 getSpeed() ;
    void setSpeed(const quint16 &value);

    qreal getViewAngle() ;
    void setViewAngle(const qreal &value);

    quint16 getViewLength() ;
    void setViewLength(const quint16 &value);

    int getTime() ;
    void setTime(int value);

    qreal getPathLength() ;
    void setPathLength(const qreal &value);

    quint16 getShipSize() ;
    void setShipSize(const quint16 &value);

    bool getIsViewVisible() ;
    void setIsViewVisible(bool value);

    quint16 getIsNew() ;
    void setIsNew(const quint16 &value);

    qreal getStartX() ;
    void setStartX(const qreal &value);

    qreal getStartY() ;
    void setStartY(const qreal &value);

     QVector<QPointF> points; //путь



protected:
    void advance(int phase);

private:
    quint16 id;         //индекс
    qreal courseAngle;  //курс
    quint16 speed;      //скорость
    qreal viewAngle;    //угол обзора
    quint16 viewLength; //длина обзора
    int time;           //время существования (мс)
    qreal pathLength;   //пройденный путь


    quint16 shipSize;   //размер корабля
    bool isViewVisible; //вкл/выкл область видимости

    quint16 isNew;  //новый корабль или нет
    qreal startX;   //координаты на текущем этапе
    qreal startY;   //


};

#endif // SHIPITEM_H
