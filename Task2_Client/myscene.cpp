#include "myscene.h"
#include <QtDebug>

MyScene::MyScene():QGraphicsScene(){
    isPathVisible = true;
    pathWidth = 0;
}

MyScene::~MyScene()
{

}


void MyScene::setIsPathVisible(bool value)
{
    isPathVisible = value;
}


void MyScene::setPathWidth(int value)
{
    pathWidth = value;
}


void MyScene::drawBackground(QPainter *painter, const QRectF &rect){

    QPen pen(Qt::darkGray,2+pathWidth*0.5f,Qt::DashDotLine, Qt::SquareCap,Qt::RoundJoin);
    if(pathWidth>3) pen.setStyle(Qt::SolidLine);

    painter->setPen(pen);

    //отрисовка пути, если включен чекбокс
    if (isPathVisible){
        for(int i = 0; i < shipList.size(); i++){

            for(int j = 0; j < shipList.at(i)->points.size()-1;j++){

                if (shipList.at(i)->points.size() < 2) painter->drawPoint(shipList.at(i)->points.at(j));
                else painter->drawLine(shipList.at(i)->points.at(j),shipList.at(i)->points.at(j+1));

            }
        }
    }

    else{
        painter->eraseRect(rect);
    }

    //отрисовка сетки
    painter->save();
    painter->setPen(QPen(Qt::lightGray,1));
    int l = int(rect.left());
    l -= (l % 50);

    int r = int(rect.right());
    r -= (r % 50);
    if(r < int(rect.right()))
       r += 50;

    int t = int(rect.top());
    t -= (t % 50);

    int b = int(rect.bottom());
    b -= (b % 50);
    if(b < int(rect.bottom()))
       b += 50;

    for( int x = l; x <= r; x+=50) painter->drawLine(x,t,x,b);

    for( int y = t; y <= b; y+=50) painter->drawLine(l,y,r,y);
    painter->restore();
}
