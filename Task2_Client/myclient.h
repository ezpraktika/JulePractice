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
    QRect screen;

    QString host;
    int port;
    QTcpSocket *socket;
    quint16 nextBlockSize;

    QStackedWidget *txtStack;
    QPushButton *prevButton;
    QLabel *logNumber;
    QPushButton *nextButton;

    QGraphicsScene *scene;

    QVector <ShipItem*> shipList;
    quint16 shipCounter;

    QTimer* timer;

    void createGui();

private slots:
    void slotConnected();
    void slotReadyRead();
    void slotError(QAbstractSocket::SocketError err);

    void slotReactToToggleViewCheckBox(bool checked);
    void slotShipResize(int);\

    void slotConnectButton();
    void slotNextButton();
    void slotPrevButton();
};

#endif // MYCLIENT_H
