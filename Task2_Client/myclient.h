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
    QRect screen; //экран (для получения размеров)

    QString host;
    int port;
    QTcpSocket *socket;
    quint16 nextBlockSize;  //размер блока получаемых данных

    QStackedWidget *txtStack;   //стек логов кораблей
    QPushButton *prevButton;
    QLabel *logNumber;
    QPushButton *nextButton;

    QGraphicsScene *scene;  //сцена (карта)

    QVector <ShipItem*> shipList;   //вектор кораблей
    quint16 shipCounter;            //количество кораблей


    void createGui();   //создание интерфейса

private slots:
    void slotConnected();   //соединение установлено
    void slotReadyRead();   //получение данных
    void slotError(QAbstractSocket::SocketError err);   //сообщения об ошибках

    void slotReactToToggleViewCheckBox(bool checked);   //вкл/выкл область видимости
    void slotShipResize(int); //изменение размеров корабля

    void slotConnectButton();   //подключение к серверу
    void slotNextButton();      //следующий лог
    void slotPrevButton();      //предыдущий лог
};

#endif // MYCLIENT_H
