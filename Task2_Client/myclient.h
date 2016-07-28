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
#include <QSet>


#include "shipitem.h"
#include "myscene.h"
#define PERCENT_OF_SCREEN 0.5f

class MyClient : public QWidget
{
    Q_OBJECT

public:
    explicit MyClient(const QString& host, int port, QWidget *parent = 0);

    //нужен?
    ~MyClient();

private:
    bool isConnected; //флаг подключения к серверу

    QRect screen; //экран (для получения размеров)

    QString host;
    int port;
    QTcpSocket *socket;
    quint16 nextBlockSize;  //размер блока получаемых данных

    QStackedWidget *txtStack;   //стек логов кораблей
    QPushButton *prevButton;
    QLabel *logNumber;
    QPushButton *nextButton;

    QPushButton *connectButton;
    QPushButton *clearPathButton;
    QLabel *messageLabel;

    MyScene *scene;  //сцена (карта)

    QSet <quint16> idOfExistingShips;         //ID кораблей, о которых знает клиент
    quint16 shipCounter;            //количество кораблей


    QPainter* painterScene;


    void createGui();   //создание интерфейса
    void deleteShip(int num);   //удаление корабля по номеру лога
    void clearAllData();        //удаление всех данных (после дисконнекта)

private slots:
    void slotConnected();   //соединение установлено
    void slotReadyRead();   //получение данных
    void slotError(QAbstractSocket::SocketError err);   //сообщения об ошибках

    void slotReactToToggleViewCheckBox(bool checked);   //вкл/выкл область видимости
    void slotShipResize(int); //изменение размеров корабля

    void slotReactToTogglePathCheckBox(bool checked);
    void slotPathResize(int);

    void slotConnectButton();   //подключение к серверу
    void slotNextButton();      //следующий лог
    void slotPrevButton();      //предыдущий лог

    void slotClearAllPaths();
};

#endif // MYCLIENT_H
