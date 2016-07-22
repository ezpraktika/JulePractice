#ifndef MYSERVER_H
#define MYSERVER_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTextEdit>
#include <QDebug>
#include <QVBoxLayout>
#include <QLabel>

class MyServer : public QWidget
{
    Q_OBJECT

private:
    QTcpServer *server;
    QTextEdit *txt;

    //лист txt сделать
    void generateData();
    void sendAllData(QTcpSocket *socket);

public:
    explicit MyServer(int port, QWidget *parent = 0);

signals:

public slots:
    void slotNewConnection();

};

#endif // MYSERVER_H
