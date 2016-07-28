#ifndef MYSCENE_H
#define MYSCENE_H

#include <QGraphicsScene>

#include "shipitem.h"

class MyScene : public QGraphicsScene{

public:
    MyScene();
    ~MyScene();

    QVector<ShipItem *> getShipList() const;
    void setShipList(const QVector<ShipItem *> &value);

    bool getIsPathVisible() const;
    void setIsPathVisible(bool value);

    int getPathWidth() const;
    void setPathWidth(int value);

    void drawBackground(QPainter *painter, const QRectF &rect);

private:
    QVector<ShipItem*> shipList;
    bool isPathVisible;     //вкл/выкл отрисовку пути
    int pathWidth;          //модификатор ширины пути


};

#endif // MYSCENE_H
