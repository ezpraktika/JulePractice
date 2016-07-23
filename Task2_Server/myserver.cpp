#include "myserver.h"
#include "shipitemstruct.h"

MyServer::MyServer(int port, QWidget *parent) : QWidget(parent), shipCounter(0)
{
    server = new QTcpServer(this);
    if(!server->listen(QHostAddress::Any , port)){
        qDebug() << "Error\n";
        server->close();
        return;
    }

    //qDebug() << "Server started\n";
    screen = QApplication::desktop()->screenGeometry();

    connect(server, SIGNAL(newConnection()), this, SLOT(slotNewConnection()));

    createGui();

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(sendAllData()));
    timer->start(2000);
    //txt->append("server started");

}

void MyServer::createGui(){

    QPushButton *prevButton = new QPushButton("<<");
    prevButton->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    prevButton->setEnabled(false);

    QPushButton *nextButton = new QPushButton(">>");
    nextButton->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    nextButton->setEnabled(false);

    QLabel *logNumber = new QLabel("1");
    logNumber->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    QHBoxLayout *logButtonsLayout = new QHBoxLayout;
    logButtonsLayout->addWidget(prevButton);
    logButtonsLayout->insertStretch(-1);
    logButtonsLayout->addWidget(logNumber);
    logButtonsLayout->insertStretch(-1);
    logButtonsLayout->addWidget(nextButton);


    txtStack = new QStackedWidget;
    txtStack->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Expanding);
    txtStack->setFixedSize(screen.width()*PERCENT_OF_SCREEN*PERCENT_OF_SCREEN*0.7f, screen.height()*PERCENT_OF_SCREEN*0.7f);
    QTextEdit *txt = new QTextEdit;
    txt->setReadOnly(true);
    txtStack->addWidget(txt);


    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollArea->setWidget(txtStack);

    QVBoxLayout *leftPanelLayout = new QVBoxLayout();
    leftPanelLayout->addWidget(scrollArea);
    leftPanelLayout->addLayout(logButtonsLayout);


    QPushButton *createShipButton = new QPushButton ("Добавить корабль");
    connect(createShipButton,SIGNAL(clicked()),this,SLOT(slotNewShip()));
    QPushButton *deleteShipButton = new QPushButton ("Удалить корабль");

    QVBoxLayout *rightPanelLayout = new QVBoxLayout;
    rightPanelLayout->addWidget(createShipButton);
    rightPanelLayout->addWidget(deleteShipButton);
    rightPanelLayout->insertStretch(-1);

    QHBoxLayout *mainPanelLayout = new QHBoxLayout;
    mainPanelLayout->addLayout(leftPanelLayout);
    mainPanelLayout->addLayout(rightPanelLayout);

    setLayout(mainPanelLayout);
    setFixedSize(sizeHint().width(),sizeHint().height());
}

void MyServer::slotNewConnection(){
    socket = server->nextPendingConnection();
    connect(socket,SIGNAL(disconnected()),socket,SLOT(deleteLater()));

    //txt->append("client connected!!!");
    //sendAllData(socket);
}

void MyServer::slotNewShip(){

    ShipItemStruct* ship = new ShipItemStruct;
    ship->isNew=1;
    shipList.append(ship);

    if(!shipCounter){
        QTextEdit *txt = (QTextEdit*) txtStack->widget(0);
        txt->append("New Ship Created");
    }
    else{
        QTextEdit *txt = new QTextEdit;
        txt->setReadOnly(true);
        txt->append("New Ship Created");
        txtStack->addWidget(txt);
    }

    shipCounter++;
}


void MyServer::sendAllData(){
qDebug()<<"hellp";
    QByteArray block;
    QDataStream out (&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_5);

    for(int i=0; i < shipCounter; i++){

        generateData(shipList.at(i));


        out<<quint16(0) << shipList.at(i)->isNew;

        if(shipList.at(i)->isNew){
            out << shipList.at(i)->startX << shipList.at(i)->startY;
        }

        out << shipList.at(i)->id
            << shipList.at(i)->courceAngle
            << shipList.at(i)->speed
            << shipList.at(i)->viewAngle
            << shipList.at(i)->viewLength;
        //time and path

        out.device()->seek(0);
        out<<quint16(block.size()-sizeof(quint16));
        //socket->write(block);
        block.clear();
        QTextEdit *te = (QTextEdit*)txtStack->widget(i);
        te->append("id: " + shipList.at(i)->id);
        shipList.at(i)->isNew=0;
    }

}

void MyServer::generateData(ShipItemStruct *ship){

    if(ship->isNew==1){

        ship->id=shipCounter-1; //WARNIGNEINGEINGEINGE
        ship->startX=100;
        ship->startY=100;
        ship->courceAngle=0.0f;
        ship->isNew = 1;
        ship->viewAngle = 30.0f;  //не забыть стартовать время и офать isNEW
        ship->viewLength = 10;       //убрать из структуры время хотя надо переводить же етпаааиа
        ship->speed=5;
    }
    ship->courceAngle+=5.0f; //здесь генерация


}

