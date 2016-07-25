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
    qsrand(QTime::currentTime().msec());
    screen = QApplication::desktop()->screenGeometry();

    connect(server, SIGNAL(newConnection()), this, SLOT(slotNewConnection()));

    createGui();

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(sendAllData()));

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


    QVBoxLayout *leftPanelLayout = new QVBoxLayout();
    leftPanelLayout->addWidget(txtStack);
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
}

void MyServer::slotNewShip(){

    ShipItemStruct* ship = new ShipItemStruct;
    ship->isNew=1;
    shipList.append(ship);

    if(!shipCounter){
        QTextEdit *txt = (QTextEdit*) txtStack->widget(0);
        txt->append("New Ship Created");
        timer->start(100); // TIMER
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

    QByteArray block;
    QDataStream out (&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_5);

    for(int i=0; i < shipCounter; i++){

        generateData(shipList.at(i));

        out<<quint16(0) << shipList.at(i)->id << shipList.at(i)->isNew;

        if(shipList.at(i)->isNew){
            out << shipList.at(i)->startX << shipList.at(i)->startY;
        }

        out << shipList.at(i)->courseAngle
            << shipList.at(i)->speed
            << shipList.at(i)->viewAngle
            << shipList.at(i)->viewLength;
        //time and path

        out.device()->seek(0);
        out<<quint16(block.size()-sizeof(quint16));
        socket->write(block);
        block.clear();


        QTextEdit *te = (QTextEdit*)txtStack->widget(i);
        te->append(QString("Id: %1").arg(shipList.at(i)->id+1));

        if(shipList.at(i)->isNew){
            te->append(QString("Start X: %1\nStart Y: %2")
                       .arg(shipList.at(i)->startX)
                       .arg(shipList.at(i)->startY));
        }

        te->append(QString("Course angle: %1\nSpeed: %2\nView angle: %3\nViewLength: %4\nPath length: %5\nTime: %6\n")
                       .arg(shipList.at(i)->courseAngle)
                       .arg(shipList.at(i)->speed).arg(shipList.at(i)->viewAngle)
                       .arg(shipList.at(i)->viewLength).arg("later").arg("later"));

        shipList.at(i)->isNew=0;
    }

}

void MyServer::generateData(ShipItemStruct *ship){

    //если новый корабль, то задаются стартовые параметры
    if(ship->isNew==1){
        ship->id=shipCounter-1; //WARNIGNEINGEINGEINGE
        ship->startX=100;
        ship->startY=100;
        ship->courseAngle=0.0f; //SET ROTATION РАБОТАЕТ В ГРАДУСАХ
        ship->isNew = 1;
        ship->viewAngle = 35.0f;  //не забыть стартовать время и офать isNEW
        ship->viewLength = 100;   //таймер и счетчик пути!!
        ship->speed=20;
    }
    //если корабль существовал прежде
    else{

        //высчитываем следующие координаты при текущем курсе
        qreal helpX=ship->startX + qCos(qDegreesToRadians(ship->courseAngle))*ship->speed;
        qreal helpY=ship->startY + qSin(qDegreesToRadians(ship->courseAngle))*ship->speed;

        //если координаты близко к краям сцены
        if((helpX > (screen.width()*PERCENT_OF_SCREEN - 20))||(helpX<0)||(helpY>(screen.height()*PERCENT_OF_SCREEN-20))||(helpY<0)){

            if (!ship->turnAlreadyStarted){ //если разворот еще не начался

                if(qrand()%2) {         //то начинаем поворот в случайную сторону
                    ship->delta=10.0f;
                }
                else {
                    ship->delta=-10.0f;
                }
                ship->turnAlreadyStarted=true;
            }
        }
        //если координаты не приближаются к краям
        else {
            ship->turnAlreadyStarted = false;

            //если предыдущий маневр завершился
            if(ship->deltaCount<0){

                 //генерация случайного маневра
                switch(qrand()%5){

                //поворот по часовой
                case 0:
                    ship->deltaCount=qrand()%6+5;
                    ship->delta = 10.0f;
                    break;

                //поворот против часовой
                case 1:
                    ship->deltaCount=qrand()%6+5;
                    ship->delta = -10.0f;
                    break;

                //движение по прямой
                case 2:
                case 3:
                case 4:
                    ship->deltaCount=qrand()%6+5;
                    ship->delta = 0.0f;
                    break;
               }
            }
        }

        ship->courseAngle+=ship->delta; //меняем курс в соответствии с ранее выбранной дельтой
        ship->deltaCount--;

        if(ship->courseAngle>180.0f) ship->courseAngle=-180.0f;  //чтобы полный круг был
        if(ship->courseAngle<-180.0f) ship->courseAngle=+180.0f; //

        ship->startX += qCos(qDegreesToRadians(ship->courseAngle))*ship->speed; //пересчитываем координаты
        ship->startY += qSin(qDegreesToRadians(ship->courseAngle))*ship->speed; //
    }
}

