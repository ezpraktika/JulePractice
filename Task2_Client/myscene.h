#ifndef MYSCENE_H
#define MYSCENE_H

#include <QGraphicsScene>

#include "shipitem.h"

class MyScene : public QGraphicsScene{



public:
    MyScene();
    void drawBackground(QPainter *painter, const QRectF &rect);
    QVector<ShipItem*> shipList;

    bool isPathVisible;
    int pathWidth;
};

#endif // MYSCENE_H
