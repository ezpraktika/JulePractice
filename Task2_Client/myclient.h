#ifndef MYCLIENT_H
#define MYCLIENT_H

#include <QWidget>
#include <QTcpSocket>
#include <QTextEdit>
#include<QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QSlider>
#include <QScrollArea>

class MyClient : public QWidget
{
    Q_OBJECT

public:
    explicit MyClient(const QString& host, int port, QWidget *parent = 0);

    //нужен?
    ~MyClient();

private:
    QTcpSocket *socket;
    QTextEdit *txt;
    quint16 nextBlockSize;
    void createGui();

private slots:
    void slotConnected();
    void slotReadyRead();
};

#endif // MYCLIENT_H