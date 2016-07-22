#include "myserver.h"

MyServer::MyServer(int port, QWidget *parent) : QWidget(parent)/*, nextBlockSize(0)*/
{
    server = new QTcpServer(this);
    if(!server->listen(QHostAddress::Any , port)){
        qDebug() << "Error\n";
        server->close();
        return;
    }

    //qDebug() << "Server started\n";

    //МОЖНО УБРАТЬ THIS


    connect(server, SIGNAL(newConnection()), this, SLOT(slotNewConnection()));

    txt = new QTextEdit;
    txt->setReadOnly(true);
    txt->append("server started");

    //Layout setup
    QVBoxLayout* pvbxLayout = new QVBoxLayout;
    pvbxLayout->addWidget(new QLabel("<H1>Server</H1>"));
    pvbxLayout->addWidget(txt);
    setLayout(pvbxLayout);

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

