#ifndef MYCLIENT_H
#define MYCLIENT_H

#include <QWidget>
#include <QTcpSocket>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QSlider>
#include <QScrollArea>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QPen>
#include <QRect>
#include <QApplication>
#include <QDesktopWidget>
#include <QSizePolicy>
#include <QStackedWidget>
#include <QTimer>

#include "shipitem.h"

#define PERCENT_OF_SCREEN 0.5f

class MyClient : public QWidget
{
    Q_OBJECT

public:
    explicit MyClient(const QString& host, int port, QWidget *parent = 0);

    //нужен?
    ~MyClient();

private:
    QTcpSocket *socket;
    QStackedWidget *txtStack;
    quint16 nextBlockSize;
    QGraphicsScene *scene;
    QRect screen;
    QVector <ShipItem*> shipList;
    quint16 shipCounter;

    QTimer* timer;

    void createGui();

private slots:
    void slotConnected();
    void slotReadyRead();
};

#endif // MYCLIENT_H
