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

#include "shipitemstruct.h"

#define PERCENT_OF_SCREEN 0.5f

class MyServer : public QWidget
{
    Q_OBJECT

private:
    QTcpServer *server;
    QStackedWidget *txtStack;
    QRect screen;
    QTimer *timer;
    QTcpSocket *socket;
    QVector <ShipItemStruct*> shipList;
    quint16 shipCounter;
    //лист txt сделать
    void createGui();
    void generateData(ShipItemStruct *ship);


public:
    explicit MyServer(int port, QWidget *parent = 0);

signals:

public slots:
    void slotNewConnection();
    void sendAllData();
    void slotNewShip();
};

#endif // MYSERVER_H
