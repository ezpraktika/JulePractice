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

#define PERCENT_OF_SCREEN 0.5f

class MyServer : public QWidget
{
    Q_OBJECT

private:
    QTcpServer *server;
    QStackedWidget *txtStack;
    QRect screen;

    //лист txt сделать
    void createGui();
    void generateData();
    void sendAllData(QTcpSocket *socket);

public:
    explicit MyServer(int port, QWidget *parent = 0);

signals:

public slots:
    void slotNewConnection();

};

#endif // MYSERVER_H
