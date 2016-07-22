#include "myclient.h"
#include "ui_myclient.h"

MyClient::MyClient(const QString& host, int port, QWidget *parent) :
    QWidget(parent),
    nextBlockSize(0),
    ui(new Ui::MyClient)
{
    socket=new QTcpSocket(this);
    socket->connectToHost(host,port);
    connect(socket,SIGNAL(connected()),SLOT(slotConnected()));

    connect(socket, SIGNAL(readyRead()), SLOT(slotReadyRead()));
    /*connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),
    this, SLOT(slotError(QAbstractSocket::SocketError))
    );*/
    txt=new QTextEdit;
    txt->setReadOnly(true);


    //Layout setup
    QVBoxLayout* pvbxLayout = new QVBoxLayout;
    pvbxLayout->addWidget(new QLabel("<H1>Client</H1>"));
    pvbxLayout->addWidget(txt);

    setLayout(pvbxLayout);


    ui->setupUi(this);
}

void MyClient::slotReadyRead()
{
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_5);
    for (;;) {
        if (!nextBlockSize) {
                if (socket->bytesAvailable() < sizeof(quint16)) {
                break;
            }
            in >> nextBlockSize;
        }
        if (socket->bytesAvailable() < nextBlockSize) {
            break;
        }
        quint16 a;
        quint16 b;
        quint16 c;
        QString str;
        in >> a >> b>>c>>str;
        txt->append(""+ QString(a) +" "+QString(b)+" "+QString(c)+" "+str);
        nextBlockSize = 0;
    }
}

void MyClient::slotConnected()
{
    qDebug()<<"Received the connected() signal";
}

MyClient::~MyClient()
{
    delete ui;
}
