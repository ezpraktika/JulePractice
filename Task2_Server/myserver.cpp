#include "myserver.h"
#include "shipitemstruct.h"

MyServer::MyServer(int port, QWidget *parent) : QWidget(parent), shipCounter(0), shipIndexCounter(0)
{
    server = new QTcpServer(this);
    if(!server->listen(QHostAddress::Any , port)){
        messageLabel -> setText("Port error");
        server->close();
        return;
    }

    isClientConnected = false;

    qsrand(QTime::currentTime().msec());
    screen = QApplication::desktop()->screenGeometry();

    connect(server, SIGNAL(newConnection()), this, SLOT(slotNewConnection()));

    createGui();

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(sendAllData()));

}

MyServer::~MyServer()
{

}

//создание интерфейса
void MyServer::createGui(){

    //панель управления логом
    prevButton = new QPushButton("<<",this);
    prevButton->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    prevButton->setEnabled(false);
    connect(prevButton, SIGNAL(clicked(bool)),this,SLOT(slotPrevButton()));

    nextButton = new QPushButton(">>",this);
    nextButton->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    nextButton->setEnabled(false);
    connect(nextButton,SIGNAL(clicked(bool)),this,SLOT(slotNextButton()));

    logNumber = new QLabel("1",this);
    logNumber->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    QHBoxLayout *logButtonsLayout = new QHBoxLayout;
    logButtonsLayout->addWidget(prevButton);
    logButtonsLayout->insertStretch(-1);
    logButtonsLayout->addWidget(logNumber);
    logButtonsLayout->insertStretch(-1);
    logButtonsLayout->addWidget(nextButton);

    //лог
    txtStack = new QStackedWidget(this);
    txtStack->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Expanding);
    txtStack->setFixedSize(screen.width()*PERCENT_OF_SCREEN*PERCENT_OF_SCREEN*0.7f, screen.height()*PERCENT_OF_SCREEN*0.7f);
    QTextEdit *txt = new QTextEdit(txtStack);
    txt->setReadOnly(true);
    txtStack->addWidget(txt);

    //левая панель сервера (лог и кнопки)
    QVBoxLayout *leftPanelLayout = new QVBoxLayout();
    leftPanelLayout->addWidget(txtStack);
    leftPanelLayout->addLayout(logButtonsLayout);

    //правая панель (кнопки создания и удаления кораблей)
    QPushButton *createShipButton = new QPushButton ("Добавить корабль",this);
    createShipButton->setFixedWidth(createShipButton->sizeHint().width()*1.3);
    connect(createShipButton,SIGNAL(clicked()),this,SLOT(slotNewShip()));

    deleteShipButton = new QPushButton ("Удалить корабль",this);
    deleteShipButton->setFixedWidth(createShipButton->sizeHint().width()*1.3);
    deleteShipButton->setEnabled(false);
    connect(deleteShipButton,SIGNAL(clicked(bool)),this,SLOT(slotDeleteShip()));

    messageLabel=new QLabel("",this);


    QVBoxLayout *rightPanelLayout = new QVBoxLayout;
    rightPanelLayout->addWidget(createShipButton);
    rightPanelLayout->addWidget(deleteShipButton);
    rightPanelLayout->addWidget(messageLabel);
    rightPanelLayout->insertStretch(-1);


    QHBoxLayout *mainPanelLayout = new QHBoxLayout;
    mainPanelLayout->addLayout(leftPanelLayout);
    mainPanelLayout->addLayout(rightPanelLayout);

    setLayout(mainPanelLayout);
    setFixedSize(sizeHint().width(),sizeHint().height());
}

//слот - новое подключение
void MyServer::slotNewConnection(){
    socket = server->nextPendingConnection();
    connect (socket,SIGNAL(disconnected()),this,SLOT(slotClientDisconnected()));
    connect(socket,SIGNAL(disconnected()),socket,SLOT(deleteLater()));
    isClientConnected = true;
    messageLabel->setText("Client connected");
}

//слот - дисконнект клиента
void MyServer::slotClientDisconnected(){
    messageLabel -> setText("Client disconnected");
    isClientConnected = false;
    socket->close();
}

//слот - создание нового корабля
void MyServer::slotNewShip(){

    deleteShipButton->setEnabled(false);

    //создание корабля
    ShipItemStruct* ship = new ShipItemStruct;
    ship->isNew=1;
    shipList.append(ship);

    //если это первый корабль
    if(!shipCounter){
        QTextEdit *txt = (QTextEdit*) txtStack->widget(0);
        txt->append("Ship created");


        //shipCounter++;
        timer->start(500); // TIMER
    }

    //если не первый корабль
    else{
        //создаем новый лог
        QTextEdit *txt = new QTextEdit;
        txt->setReadOnly(true);
        txt->append("Ship created");

        txtStack->addWidget(txt);
        nextButton->setEnabled(true);
    }
    shipCounter++;
}

//слот - удаление корабля
void MyServer::slotDeleteShip(){

    //считываем номер текущей страницы
    //(номер удаляемого корабля)
    int num = logNumber->text().toInt();

    //получаем адрес лога удаляемого корабля
    QTextEdit *txt = (QTextEdit*) txtStack->widget(num-1);

    int idOfDeleted = shipList.at(num-1)->id;

    //удаляем корабль из вектора
    shipList.remove(num-1);

    shipCounter--;

    //если это был последний корабль
    if(shipCounter==0){
        txt->clear();                           //очищаем лог
        deleteShipButton->setEnabled(false);    //отключаем кнопку удаления
    }
    //иначе
    else{

        txtStack->removeWidget(txt);  //удаляем лог

        if(num > shipCounter){                            //если номер лога больше количества кораблей
            logNumber->setText(QString::number(num-1));   //обновляем номер
            txtStack->setCurrentIndex(num-2);
        }
        else{
            txtStack->setCurrentIndex(num-1);
        }

        delete txt;
    }

    //если в итоге оказались на первом логе
    if(!txtStack->currentIndex()){
        prevButton->setEnabled(false);
    }

    //если в итоге оказались на последнем логе
    if((txtStack->currentIndex()+1)==shipCounter){
        nextButton->setEnabled(false);
    }

    messageLabel->setText(QString("Ship (ID: %1) - deleted").arg(idOfDeleted+1));
    logNumbersOfRemovedShips.append(num);         //добавить в список номер лога удаленного корабля

}

//слот - переход к логу следующего корабля
void MyServer::slotNextButton()
{
    //считываем номер текущей страницы
    int num = logNumber->text().toInt();

    //если это первая страница, то включаем кнопку "<<"
    if (num == 1) prevButton->setEnabled(true);

    //стек выбирает следующий textEdit для показа
    txtStack->setCurrentIndex(num);

    //новый номер страницы
    logNumber->setText(QString::number(++num));

    //если это последняя страница, отключаем кнопку ">>"
    if(num == shipCounter) nextButton->setEnabled(false);

}

//слот - переход к логу предыдущего корабля
void MyServer::slotPrevButton()
{
    //считываем номер текущей страницы
    int num = logNumber->text().toInt();

    //если это последняя страница, то включаем кнопку ">>"
    if (num == shipCounter) nextButton->setEnabled(true);

    //стек выбирает предыдущий textEdit для показа
    txtStack->setCurrentIndex(num-2);

    //новый номер страницы
    logNumber->setText(QString::number(--num));

    //если это первая страница, отключаем кнопку "<<"
    if(num == 1) prevButton->setEnabled(false);
}

//отправление данных по всем кораблям
void MyServer::sendAllData(){
    //deleteShipButton->setEnabled(false);

    QByteArray block;
    QDataStream out (&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_5);

    out << quint16(0) << logNumbersOfRemovedShips.size();       //количество удаленных кораблей
    for(int k = 0; k < logNumbersOfRemovedShips.size(); k++){   //номер лога удаленного корабля
        out << logNumbersOfRemovedShips.at(k);                  //
    }

    logNumbersOfRemovedShips.clear();

    out << shipCounter;     //количество существующих на сервере кораблей

    //для всех кораблей
    for(int i=0; i < shipCounter; i++){

        generateData(shipList.at(i));   //генерируем новые данные

        out << shipList.at(i)->id
            << shipList.at(i)->startX
            << shipList.at(i)->startY
            << shipList.at(i)->courseAngle
            << shipList.at(i)->speed
            << shipList.at(i)->viewAngle
            << shipList.at(i)->viewLength
            << shipList.at(i)->pathLength
            << shipList.at(i)->time;



        QTextEdit *te = (QTextEdit*)txtStack->widget(i);        //получение указателя на лог текущего корабля
        te->append(QString("Id: %1").arg(shipList.at(i)->id+1));//вывод сгенерированной информации в лог

        if(shipList.at(i)->isNew){
            te->append(QString("Start X: %1\nStart Y: %2")
                       .arg(shipList.at(i)->startX)
                       .arg(shipList.at(i)->startY));
        }



        te->append(QString("Course angle: %1 deg\nSpeed: %2\nView angle: %3 deg\nViewLength: %4\nPath length: %5 m\nTime: %6 sec\n")
                       .arg(shipList.at(i)->courseAngle)
                       .arg(shipList.at(i)->speed).arg(shipList.at(i)->viewAngle)
                       .arg(shipList.at(i)->viewLength).arg(shipList.at(i)->pathLength).arg(shipList.at(i)->time/1000.0f));

        shipList.at(i)->isNew=0;
    }

    out.device()->seek(0);  //переход в начало блока
    out<<quint16(block.size()-sizeof(quint16)); //размер блока данных
    if(isClientConnected) socket->write(block);   //посылка клиенту, если он подключен
    block.clear();          //очистка используемого блока

    if(!deleteShipButton->isEnabled()&& shipCounter>0) deleteShipButton->setEnabled(true);
}

//генерация новых данных для корабля
void MyServer::generateData(ShipItemStruct *ship){

    //если новый корабль, то задаются стартовые параметры
    if(ship->isNew==1){
        ship->startX=(qrand()% (int) (screen.width()*PERCENT_OF_SCREEN-200))+100;
        ship->startY=(qrand()%  (int) (screen.height()*PERCENT_OF_SCREEN-200))+100;
        ship->id=shipIndexCounter;
        ship->courseAngle= (qreal) (qrand() % 361) - 180.0f;
        ship->speed=qrand()%21 + 15;
        ship->viewAngle = 35.0f;
        ship->viewLength = 100;
        ship->timer.start();
        ship->time=0;
        ship->pathLength=0;

        ship->delta=0.0f;
        ship->deltaCount=-1;

        shipIndexCounter++;
    }
    //если корабль существовал прежде
    else{

        //высчитываем следующие координаты при текущем курсе
        qreal helpX=ship->startX + qCos(qDegreesToRadians(ship->courseAngle))*ship->speed;
        qreal helpY=ship->startY + qSin(qDegreesToRadians(ship->courseAngle))*ship->speed;

        //если координаты близко к краям сцены
        if((helpX > (screen.width()*PERCENT_OF_SCREEN - 40))||(helpX<40)||(helpY>(screen.height()*PERCENT_OF_SCREEN-40))||(helpY<40)){

            if (!ship->turnAlreadyStarted){ //если разворот еще не начался

                if(qrand()%2) {         //то начинаем поворот в случайную сторону
                    ship->delta=25.0f;
                }
                else {
                    ship->delta=-25.0f;
                }
                ship->deltaCount=-1;
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
                    ship->deltaCount=qrand()%4+2;
                    ship->delta = 10.0f;
                    break;

                //поворот против часовой
                case 1:
                    ship->deltaCount=qrand()%4+2;
                    ship->delta = -10.0f;
                    break;

                //движение по прямой
                case 2:
                case 3:
                case 4:
                    ship->deltaCount=qrand()%4+2;
                    ship->delta = 0.0f;
                    break;
               }
            }
        }

        ship->courseAngle+=ship->delta; //меняем курс в соответствии с ранее выбранной дельтой
        ship->deltaCount--;

        if(ship->courseAngle>180.0f) {
            ship->courseAngle=-180.0f;  //чтобы полный круг был
        }
        if(ship->courseAngle<-180.0f) ship->courseAngle=+180.0f; //

        ship->pathLength += sqrtf((helpX-ship->startX)*(helpX-ship->startX)+(helpY-ship->startY)*(helpY-ship->startY));

        ship->startX += qCos(qDegreesToRadians(ship->courseAngle))*ship->speed; //пересчитываем координаты
        ship->startY += qSin(qDegreesToRadians(ship->courseAngle))*ship->speed; //

        ship->time = ship->timer.elapsed();
    }
}

