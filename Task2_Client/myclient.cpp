#include "myclient.h"

MyClient::MyClient(const QString& host, int port, QWidget *parent) :
    QWidget(parent),
    nextBlockSize(0),
    shipCounter(0)
{
    screen = QApplication::desktop()->screenGeometry();

    this->host=host;
    this->port=port;

    createGui();
}

//генерация интерфейса
void MyClient::createGui(){

    //панель отрисовки

    scene = new QGraphicsScene;
    QGraphicsView *view = new QGraphicsView(scene);

    view->setRenderHint(QPainter::Antialiasing);
    view->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    scene->setSceneRect(0,0,screen.width()*PERCENT_OF_SCREEN,screen.height()*PERCENT_OF_SCREEN);

    QPen pen = QPen(Qt::red);

    scene->addRect(scene->sceneRect(), pen);

    QRectF turnArea = QRectF(QPointF(40,40),QPointF(screen.width()*PERCENT_OF_SCREEN - 40,screen.height()*PERCENT_OF_SCREEN - 40));
    scene->addRect(turnArea,pen);

    //панель управления отрисовкой

    connectButton = new QPushButton("Connect");
    connectButton->setFixedWidth(connectButton->sizeHint().width()*2);
    connect(connectButton,SIGNAL(clicked(bool)),this,SLOT(slotConnectButton()));
    connect(connectButton,SIGNAL(clicked(bool)),connectButton,SLOT(setEnabled(bool)));
    messageLabel = new QLabel;


    QCheckBox *showPathCheckBox = new QCheckBox("Путь");
    QCheckBox *showViewCheckBox = new QCheckBox("Угол обзора");
    connect(showViewCheckBox,SIGNAL(toggled(bool)),this,SLOT(slotReactToToggleViewCheckBox(bool)));
    showViewCheckBox->setChecked(true);

    QLabel *shipSizeLabel = new QLabel("Размер корабля");
    QLabel *pathSizeLabel = new QLabel("Ширина пути");

    QSlider *shipSizeSlider = new QSlider(Qt::Horizontal);
    shipSizeSlider->setMaximumWidth(120);
    shipSizeSlider->setMinimum(1);
    shipSizeSlider->setMaximum(5);
    connect(shipSizeSlider,SIGNAL(valueChanged(int)),this,SLOT(slotShipResize(int)));

    QSlider *pathSizeSlider = new QSlider(Qt::Horizontal);
    pathSizeSlider->setMaximumWidth(120);

    QVBoxLayout *connectLayout = new QVBoxLayout;
    connectLayout->addWidget(connectButton);
    connectLayout->addWidget(messageLabel);

    QVBoxLayout *checkBoxLayout = new QVBoxLayout;
    checkBoxLayout->addWidget(showPathCheckBox);
    checkBoxLayout->addWidget(showViewCheckBox);

    QVBoxLayout *labelLayout = new QVBoxLayout;
    labelLayout->addWidget(shipSizeLabel);
    labelLayout->addWidget(pathSizeLabel);

    QVBoxLayout *sliderLayout = new QVBoxLayout;
    sliderLayout->addWidget(shipSizeSlider);
    sliderLayout->addWidget(pathSizeSlider);

    QHBoxLayout *controlButtons= new QHBoxLayout;
    controlButtons->addLayout(connectLayout);
    controlButtons->insertStretch(-1);
    controlButtons->addLayout(checkBoxLayout);
    controlButtons->insertStretch(-1);
    controlButtons->addLayout(labelLayout);
    controlButtons->addLayout(sliderLayout);
    controlButtons->insertStretch(-1);

    //левая часть окна

    QVBoxLayout *leftPanelLayout = new QVBoxLayout;
    leftPanelLayout->addWidget(view);
    leftPanelLayout->addLayout(controlButtons);

    //панель управления логами

    prevButton = new QPushButton("<<");
    prevButton->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    prevButton->setEnabled(false);
    connect(prevButton, SIGNAL(clicked(bool)),this,SLOT(slotPrevButton()));

    nextButton = new QPushButton(">>");
    nextButton->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    nextButton->setEnabled(false);
    connect(nextButton,SIGNAL(clicked(bool)),this,SLOT(slotNextButton()));

    logNumber = new QLabel("1");
    logNumber->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    QHBoxLayout *logButtonsLayout = new QHBoxLayout;
    logButtonsLayout->addWidget(prevButton);
    logButtonsLayout->insertStretch(-1);
    logButtonsLayout->addWidget(logNumber);
    logButtonsLayout->insertStretch(-1);
    logButtonsLayout->addWidget(nextButton);

    //поле логов

    txtStack = new QStackedWidget;
    txtStack->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Expanding);
    QTextEdit *txt = new QTextEdit;
    txt->setReadOnly(true);
    txtStack->addWidget(txt);


    //правая часть окна

    QVBoxLayout *rightPanelLayout = new QVBoxLayout();
    rightPanelLayout->addWidget(txtStack);
    rightPanelLayout->addLayout(logButtonsLayout);

    //интерфейс целиком

    QHBoxLayout *mainPanelLayout = new QHBoxLayout;

    mainPanelLayout->addLayout(leftPanelLayout);
    mainPanelLayout->addLayout(rightPanelLayout);

    setLayout(mainPanelLayout);
    setFixedSize(sizeHint().width(),sizeHint().height());


}

void MyClient::slotReadyRead()
{
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_5);

    //в бесконечном цикле считываем данные

    for (;;) {
        //если не знаем размер входящего блока
        if (!nextBlockSize) {
                if (socket->bytesAvailable() < sizeof(quint16)) {   //если не можем считать размер блока,
                break;                                              //то выходим
            }
            in >> nextBlockSize;    //иначе - считываем размер блока
        }

        //если из сокета нельзя прочитать весь блок - выходим
        if (socket->bytesAvailable() < nextBlockSize) {
            break;
        }

        //количество кораблей, данные о которых генерируются на сервере
        quint16 serverShipCounter, inputIsNew, inputID;

        int numOfRemovedShips;

        in >> numOfRemovedShips;

        if(numOfRemovedShips){
            quint16 removeLogNumber;
            for(int k = 0; k < numOfRemovedShips; k++){
                in >> removeLogNumber;
                deleteShip(removeLogNumber);
            }
        }

        in >> serverShipCounter;



        for(int j = 0;j<serverShipCounter;j++){
            in >> inputID >> inputIsNew;

            //если данные о новом корабле
            if(inputIsNew){
                ShipItem *ship = new ShipItem;  //создаем новый корабль
                shipList.append(ship);          //помещаем его в вектор
                scene->addItem(ship);           //и добавляем на сцену

                shipCounter++;          //количество кораблей увеличилось

                //если это был не первый добавленный корабль
                if(shipCounter > 1){
                    QTextEdit *txt = new QTextEdit; //создаем лог для нового корабля
                    txt->setReadOnly(true);         //
                    txtStack->addWidget(txt);       //и добавляем в стек виджетов
                    qDebug()<<"in is new";
                    qDebug()<<"ship count " <<shipCounter;
                    nextButton->setEnabled(true);
                }

                in >> shipList.at(j)->startX  //считываем начальные координаты
                        >> shipList.at(j)->startY; //


            }


            //помещаем в корабль считанные ранее параметры
            shipList.at(j)->id = inputID;
            shipList.at(j)->isNew = inputIsNew;

            //и считываем остальные
            in >> shipList.at(j)->courseAngle
                    >> shipList.at(j)->speed
                    >> shipList.at(j)->viewAngle
                    >> shipList.at(j)->viewLength
                    >> shipList.at(j)->pathLength
                    >> shipList.at(j)->time;

            //получаем указатель на нужный лог и записываем в него все считанные данные
            QTextEdit *te = (QTextEdit*)txtStack->widget(j);
            te->append(QString("Id: %1").arg(shipList.at(j)->id+1));

            if(shipList.at(j)->isNew){
                te->append(QString("Start X: %1\nStart Y: %2")
                           .arg(shipList.at(j)->startX)
                           .arg(shipList.at(j)->startY));
            }

            te->append(QString("Course angle: %1\nSpeed: %2\nView angle: %3\nViewLength: %4\nPath length: %5\nTime: %6\n")
                       .arg(shipList.at(j)->courseAngle)
                       .arg(shipList.at(j)->speed).arg(shipList.at(j)->viewAngle)
                       .arg(shipList.at(j)->viewLength).arg(shipList.at(j)->pathLength).arg(shipList.at(j)->time));
        }
        //новый блок данных
        nextBlockSize = 0;
    }
    scene->advance();

}

//вкл/выкл область видимости
void MyClient::slotReactToToggleViewCheckBox(bool checked)
{

    ShipItem* a = (ShipItem*)scene->items().at(0);
    a->isViewVisible = checked;
}

//подключиться к серверу
void MyClient::slotConnectButton(){
    socket=new QTcpSocket(this);
    socket->connectToHost(host,port);

    connect(socket,SIGNAL(connected()),SLOT(slotConnected()));
    connect(socket, SIGNAL(readyRead()), SLOT(slotReadyRead()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(slotError(QAbstractSocket::SocketError)));
}

//соединение установлено
void MyClient::slotConnected()
{
    messageLabel->setText("Connected");
    connectButton->setText("Disconnect");
    connectButton->setEnabled(true);
}


//сообщения об ошибках
void MyClient::slotError(QAbstractSocket::SocketError err)
{
    switch(err){
    case QAbstractSocket::HostNotFoundError:
        messageLabel->setText("The host was not found");
        break;
    case QAbstractSocket::RemoteHostClosedError:
        messageLabel->setText("The remote host is closed");
        break;
    case QAbstractSocket::ConnectionRefusedError:
        messageLabel->setText("The connection was refused");
        connectButton->setEnabled(true);
        break;
    default:
        messageLabel->setText(QString(socket->errorString()));
        break;
    }


}


//изменение размеров корабля
void MyClient::slotShipResize(int val){
    ShipItem* a = (ShipItem*)scene->items().at(0);
    a->shipSize = val;
}

//следующий лог
void MyClient::slotNextButton()
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

//предыдущий лог
void MyClient::slotPrevButton()
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

//удаление корабля с номером лога num
void MyClient::deleteShip(int num)
{
    int currentLogNumber = logNumber->text().toInt(); //показываемый в клиенте лог

    //получаем адрес лога удаляемого корабля
    QTextEdit *txt = (QTextEdit*) txtStack->widget(num-1);

    int idOfDeleted = shipList.at(num-1)->id;

    //удаляем корабль из сцены и вектора
    ShipItem *ship = shipList.at(num-1);
    scene->removeItem(ship);
    shipList.remove(num-1);
    delete ship;

    shipCounter--;

    //если это был последний корабль
    if(shipCounter==0){
        txt->clear();   //очищаем лог
    }
    //иначе
    else{

        txtStack->removeWidget(txt);  //удаляем лог

        if(currentLogNumber > shipCounter){                            //если номер лога больше количества кораблей
            logNumber->setText(QString::number(currentLogNumber-1));   //обновляем номер
            txtStack->setCurrentIndex(currentLogNumber-2);
        }
        else{
            txtStack->setCurrentIndex(currentLogNumber-1);
        }

        delete txt; //очистка памяти
    }

    //если в итоге оказались на первом логе
    if(!txtStack->currentIndex()){
        prevButton->setEnabled(false);
    }

    //если в итоге оказались на последнем логе
    if((txtStack->currentIndex()+1)==shipCounter){
        nextButton->setEnabled(false);
    }

    messageLabel->setText(QString("Ship (ID: %1) has been deleted").arg(idOfDeleted+1));


    //удалить объект со сцены

}


MyClient::~MyClient()
{

}
