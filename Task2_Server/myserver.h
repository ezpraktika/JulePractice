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
   // quint16 nextBlockSize;
    //лист txt сделать
   // void sendToClient(QTcpSocket* socket, const QString& message);
    void generateData();
    void sendAllData(QTcpSocket *socket);
public:
    explicit MyServer(int port, QWidget *parent = 0);

signals:

public slots:
    void slotNewConnection();

};

#endif // MYSERVER_H
