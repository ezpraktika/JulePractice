#ifndef MYCLIENT_H
#define MYCLIENT_H

#include <QWidget>
#include <QTcpSocket>
#include <QTextEdit>
#include<QVBoxLayout>
#include <QLabel>

namespace Ui {
class MyClient;
}

class MyClient : public QWidget
{
    Q_OBJECT

public:
    explicit MyClient(const QString& host, int port, QWidget *parent = 0);
    ~MyClient();

private:
    QTcpSocket *socket;
    QTextEdit *txt;
    quint16 nextBlockSize;
    Ui::MyClient *ui;

private slots:
    void slotConnected();
    void slotReadyRead();
};

#endif // MYCLIENT_H
