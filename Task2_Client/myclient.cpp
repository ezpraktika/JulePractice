#include "myclient.h"

MyClient::MyClient(const QString& host, int port, QWidget *parent) :
    QWidget(parent),
    nextBlockSize(0)
{
    socket=new QTcpSocket(this);
    socket->connectToHost(host,port);

    connect(socket,SIGNAL(connected()),SLOT(slotConnected()));
    connect(socket, SIGNAL(readyRead()), SLOT(slotReadyRead()));
    /*connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),
    this, SLOT(slotError(QAbstractSocket::SocketError))
    );*/
    createGui();
    txt->append("client created");

}

void MyClient::createGui(){

    QPushButton *connectButton = new QPushButton("Connect");

    QCheckBox *showPathCheckBox = new QCheckBox("Путь");
    QCheckBox *showViewCheckBox = new QCheckBox("Угол обзора");

    QLabel *shipSizeLabel = new QLabel("Размер корабля");
    QLabel *pathSizeLabel = new QLabel("Ширина пути");

    QSlider *shipSizeSlider = new QSlider(Qt::Horizontal);
    QSlider *pathSizeSlider = new QSlider(Qt::Horizontal);

    QGridLayout *controlButtonsLayout = new QGridLayout;
    controlButtonsLayout->addWidget(connectButton,0,0);
    controlButtonsLayout->addWidget(showPathCheckBox,0,1);
    controlButtonsLayout->addWidget(shipSizeLabel,0,2);
    controlButtonsLayout->addWidget(shipSizeSlider,0,3);
    controlButtonsLayout->addWidget(showViewCheckBox,1,1);
    controlButtonsLayout->addWidget(pathSizeLabel,1,2);
    controlButtonsLayout->addWidget(pathSizeSlider,1,3);


    QVBoxLayout *leftPanelLayout = new QVBoxLayout;
    leftPanelLayout->addLayout(controlButtonsLayout);


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

    QVBoxLayout *rightPanelLayout = new QVBoxLayout();
    rightPanelLayout->addWidget(scrollArea);
    rightPanelLayout->addLayout(logButtonsLayout);

    QHBoxLayout *mainPanelLayout = new QHBoxLayout;
    mainPanelLayout->addLayout(leftPanelLayout);
    mainPanelLayout->addLayout(rightPanelLayout);

    setLayout(mainPanelLayout);
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

        QString str;
        in >> str;
        qDebug()<<str;
        txt->append("new data: " + str);
        nextBlockSize = 0;
    }
}

void MyClient::slotConnected()
{
    txt->append("Received the connected() signal");
}

MyClient::~MyClient()
{

}
