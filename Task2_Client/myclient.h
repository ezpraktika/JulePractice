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

    ~MyClient();

private:
    bool isConnected; //флаг подключения к серверу

    QRect screen; //экран (для получения размеров)

    QString host;
    int port;
    QTcpSocket *socket;
    quint16 nextBlockSize;  //размер блока получаемых данных

    QStackedWidget *txtStack;   //стек логов кораблей

    QPushButton *prevButton;    //панель управления логами
    QLabel *logNumber;          //
    QPushButton *nextButton;    //

    QPushButton *connectButton;     //кнопка подключения
    QPushButton *clearPathButton;   //кнопка очистки путей

    QCheckBox *showViewCheckBox;    //вкл/выкл область видимости
    QSlider *shipSizeSlider;    //слайдер размеров корабля

    QLabel *messageLabel;   //панель сообщений

    MyScene *scene;  //сцена (карта)

    QSet <quint16> idOfExistingShips;         //ID кораблей, о которых знает клиент
    quint16 shipCounter;                      //количество кораблей


    QPainter* painterScene;


    void createGui();           //создание интерфейса
    void deleteShip(int num);   //удаление корабля по номеру лога
    void clearAllData();        //удаление всех данных (после дисконнекта)

private slots:
    void slotConnected();   //соединение установлено
    void slotReadyRead();   //получение данных
    void slotError(QAbstractSocket::SocketError err);   //сообщения об ошибках

    void slotReactToToggleViewCheckBox(bool checked);   //вкл/выкл область видимости
    void slotShipResize(int);                           //изменение размеров корабля

    void slotReactToTogglePathCheckBox(bool checked);   //вкл/выкл пути
    void slotPathResize(int);                           //изменение ширины пути

    void slotConnectButton();   //подключение к серверу
    void slotNextButton();      //следующий лог
    void slotPrevButton();      //предыдущий лог

    void slotClearAllPaths();   //очистка путей
};

#endif // MYCLIENT_H
