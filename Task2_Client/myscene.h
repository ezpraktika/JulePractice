#ifndef MYSCENE_H
#define MYSCENE_H

#include <QGraphicsScene>

#include "shipitem.h"

class MyScene : public QGraphicsScene{

public:
    MyScene();
    ~MyScene();

     QVector<ShipItem*> shipList;

    void setIsPathVisible(bool value);
    void setPathWidth(int value);

    void drawBackground(QPainter *painter, const QRectF &rect);

private:

    bool isPathVisible;     //вкл/выкл отрисовку пути
    int pathWidth;          //модификатор ширины пути


};

#endif // MYSCENE_H
