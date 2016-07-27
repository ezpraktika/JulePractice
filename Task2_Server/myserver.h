#ifndef MYSERVER_H
#define MYSERVER_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTextEdit>
#include <QDebug>
#include <QVBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <QPushButton>
#include <QRect>
#include <QApplication>
#include <QDesktopWidget>
#include <QSizePolicy>
#include <QStackedWidget>
#include <QTimer>
#include <QVector>
#include <QtMath>


#include "shipitemstruct.h"

#define PERCENT_OF_SCREEN 0.5f

class MyServer : public QWidget
{
    Q_OBJECT

private:
    QRect screen;   //экран (для получения размеров)

    QTcpServer *server;
    QTcpSocket *socket;

    QStackedWidget *txtStack;   //стек для логов кораблей
    QPushButton *prevButton;
    QLabel *logNumber;
    QPushButton *nextButton;

    QPushButton *deleteShipButton;

    QLabel *messageLabel;

    QTimer *timer;

    QVector <ShipItemStruct*> shipList; //вектор указателей на корабли
    quint16 shipCounter;        //количество кораблей
    quint16 shipIndexCounter;   //для неповторяющейся индексации кораблей
    QVector <quint16> logNumbersOfRemovedShips; //индексы удаленных кораблей

    void createGui();                           //создание интерфейса
    void generateData(ShipItemStruct *ship);    //генерация данных

public:
    explicit MyServer(int port, QWidget *parent = 0);

signals:

public slots:
    void slotNewConnection();   //новое соединение
    void sendAllData();         //отправка данных по всем кораблям
    void slotNewShip();         //создание корабля
    void slotDeleteShip();      //удаление корабля
    void slotNextButton();      //следующий лог
    void slotPrevButton();      //предыдущий лог
};

#endif // MYSERVER_H
