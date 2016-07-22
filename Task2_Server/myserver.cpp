#include "myserver.h"

MyServer::MyServer(int port, QWidget *parent) : QWidget(parent)
{
    server = new QTcpServer(this);
    if(!server->listen(QHostAddress::Any , port)){
        qDebug() << "Error\n";
        server->close();
        return;
    }

    //qDebug() << "Server started\n";

    connect(server, SIGNAL(newConnection()), this, SLOT(slotNewConnection()));

    createGui();

}

void MyServer::createGui(){

    QPushButton *prevButton = new QPushButton("<<");
    prevButton->setEnabled(false);

    QPushButton *nextButton = new QPushButton(">>");
    nextButton->setEnabled(false);

    QLabel *logNumber = new QLabel("1");

    QHBoxLayout *logButtonsLayout = new QHBoxLayout;
    logButtonsLayout->addWidget(prevButton);
    logButtonsLayout->addWidget(logNumber);
    logButtonsLayout->addWidget(nextButton);


    txt = new QTextEdit();
    txt->setReadOnly(true);

    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollArea->setWidget(txt);

    QVBoxLayout *leftPanelLayout = new QVBoxLayout();
    leftPanelLayout->addWidget(scrollArea);
    leftPanelLayout->addLayout(logButtonsLayout);


    QPushButton *createShipButton = new QPushButton ("Добавить корабль");
    QPushButton *deleteShipButton = new QPushButton ("Удалить корабль");

    QVBoxLayout *rightPanelLayout = new QVBoxLayout;
    rightPanelLayout->addWidget(createShipButton);
    rightPanelLayout->addWidget(deleteShipButton);

    QHBoxLayout *mainPanelLayout = new QHBoxLayout;
    mainPanelLayout->addLayout(leftPanelLayout);
    mainPanelLayout->addLayout(rightPanelLayout);

    setLayout(mainPanelLayout);

}

void MyServer::slotNewConnection(){
    QTcpSocket* socket = server->nextPendingConnection();
    connect(socket,SIGNAL(disconnected()),socket,SLOT(deleteLater()));


    txt->append("connected!!!");
    sendAllData(socket);
}
void MyServer::sendAllData(QTcpSocket *socket){

    QByteArray block;
    QDataStream out (&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_5);
    //рандомы по данным -
    //foreach void generateData(DataStruct& ds)
    //
    out<<quint16(0)<<quint16(3)<<quint16(4)<<quint16(5)<<QString("helloworld");
    out.device()->seek(0);
    out<<quint16(block.size()-sizeof(quint16));
    socket->write(block);
}

void MyServer::generateData(){


}

