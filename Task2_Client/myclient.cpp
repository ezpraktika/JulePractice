#include "myclient.h"

MyClient::MyClient(const QString& host, int port, QWidget *parent) :
    QWidget(parent),
    nextBlockSize(0),
    shipCounter(0)
{
    screen = QApplication::desktop()->screenGeometry();

    isConnected = false;

    socket=new QTcpSocket(this);

    this->host=host;
    this->port=port;

    createGui();
}

//генерация интерфейса
void MyClient::createGui(){

    //панель отрисовки

    scene = new MyScene;
    QGraphicsView *view = new QGraphicsView(scene);
    view->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    painterScene = new QPainter();

    view->setRenderHint(QPainter::Antialiasing);
    view->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    scene->setSceneRect(0,0,screen.width()*PERCENT_OF_SCREEN,screen.height()*PERCENT_OF_SCREEN);

    //панель управления отрисовкой

    connectButton = new QPushButton("Connect",this);
    connectButton->setFixedWidth(connectButton->sizeHint().width()*2);

    connect(connectButton,SIGNAL(clicked(bool)),connectButton,SLOT(setEnabled(bool)));
    connect(connectButton,SIGNAL(clicked(bool)),this,SLOT(slotConnectButton()));

    messageLabel = new QLabel("",this);

    QCheckBox *showPathCheckBox = new QCheckBox("Путь",this);
    showPathCheckBox->setChecked(true);
    connect(showPathCheckBox,SIGNAL(toggled(bool)),this,SLOT(slotReactToTogglePathCheckBox(bool)));

    showViewCheckBox = new QCheckBox("Угол обзора",this);
    showViewCheckBox->setChecked(true);
    connect(showViewCheckBox,SIGNAL(toggled(bool)),this,SLOT(slotReactToToggleViewCheckBox(bool)));

    QLabel *shipSizeLabel = new QLabel("Размер корабля",this);
    QLabel *pathSizeLabel = new QLabel("Ширина пути",this);

    shipSizeSlider = new QSlider(Qt::Horizontal,this);
    shipSizeSlider->setMaximumWidth(120);
    shipSizeSlider->setMinimum(0);
    shipSizeSlider->setMaximum(5);
    connect(shipSizeSlider,SIGNAL(valueChanged(int)),this,SLOT(slotShipResize(int)));

    QSlider *pathSizeSlider = new QSlider(Qt::Horizontal,this);
    pathSizeSlider->setMaximumWidth(120);
    pathSizeSlider->setMinimum(0);
    pathSizeSlider->setMaximum(7);
    connect(pathSizeSlider,SIGNAL(valueChanged(int)),this,SLOT(slotPathResize(int)));

    clearPathButton = new QPushButton("Стереть пути",this);
    connect(clearPathButton,SIGNAL(clicked(bool)),this,SLOT(slotClearAllPaths()));

    QVBoxLayout *connectLayout = new QVBoxLayout;
    connectLayout->addWidget(connectButton);
    connectLayout->addWidget(messageLabel);

    QVBoxLayout *checkBoxLayout = new QVBoxLayout;
    checkBoxLayout->addWidget(showPathCheckBox);
    checkBoxLayout->addWidget(showViewCheckBox);

    QVBoxLayout *labelLayout = new QVBoxLayout;
    labelLayout->addWidget(pathSizeLabel);
    labelLayout->addWidget(shipSizeLabel);


    QVBoxLayout *sliderLayout = new QVBoxLayout;
    sliderLayout->addWidget(pathSizeSlider);
    sliderLayout->addWidget(shipSizeSlider);


    QVBoxLayout *clearPathLayout = new QVBoxLayout;
    clearPathLayout->addWidget(clearPathButton);
    clearPathLayout->insertStretch(-1);

    QHBoxLayout *controlButtons= new QHBoxLayout;
    controlButtons->addLayout(connectLayout);
    controlButtons->insertStretch(-1);
    controlButtons->addLayout(checkBoxLayout);
    controlButtons->insertStretch(-1);
    controlButtons->addLayout(labelLayout);
    controlButtons->addLayout(sliderLayout);
    controlButtons->insertStretch(-1);

    controlButtons->addLayout(clearPathLayout);


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
    QTextEdit *txt = new QTextEdit(txtStack);
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

//считывание данных
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
        quint16 serverShipCounter, inputID;

        //количество удаленных кораблей
        int numOfRemovedShips;

        in >> numOfRemovedShips;

        //клиент удаляет у себя корабли, удаленные на сервере
        if(numOfRemovedShips){
            quint16 removeLogNumber;
            for(int k = 0; k < numOfRemovedShips; k++){
                in >> removeLogNumber;
                if(removeLogNumber<=shipCounter) deleteShip(removeLogNumber);  //если на клиенте изначально не было корабля,
                                                                                //который сервер удалил, то функция удаления не вызывается
            }
        }

        in >> serverShipCounter;    //количество кораблей на сервере

        for(int j = 0;j<serverShipCounter;j++){

            in >> inputID;

            bool newOnThisClient = !idOfExistingShips.contains(inputID);

            //если данные о новом корабле
            if(newOnThisClient){
                idOfExistingShips.insert(inputID);

                ShipItem *ship = new ShipItem;  //создаем новый корабль
                ship->setShipSize(shipSizeSlider->value());
                ship->setIsViewVisible(showViewCheckBox->isChecked());
                scene->shipList.append(ship);   //помещаем его в вектор
                scene->addItem(ship);           //и добавляем на сцену

                shipCounter++;          //количество кораблей увеличилось

                //если это был не первый добавленный корабль
                if(shipCounter > 1){
                    QTextEdit *txt = new QTextEdit; //создаем лог для нового корабля
                    txt->setReadOnly(true);         //
                    txtStack->addWidget(txt);       //и добавляем в стек виджетов
                    nextButton->setEnabled(true);
                }
            }

            qreal inputStartX, inputStartY, inputCourseAngle, inputViewAngle, inputPathLength;
            quint16 inputSpeed, inputViewLength;
            int inputTime;




            // считываем остальные данные
            in >> inputStartX >> inputStartY >> inputCourseAngle >> inputSpeed >> inputViewAngle >> inputViewLength
               >> inputPathLength >> inputTime;

            //и помещаем их в корабль

            scene->shipList.at(j)->setId (inputID);
            scene->shipList.at(j)->setIsNew (newOnThisClient);
            scene->shipList.at(j)->setStartX (inputStartX);
            scene->shipList.at(j)->setStartY (inputStartY);
            scene->shipList.at(j)->setCourseAngle (inputCourseAngle);
            scene->shipList.at(j)->setSpeed (inputSpeed);
            scene->shipList.at(j)->setViewAngle (inputViewAngle);
            scene->shipList.at(j)->setViewLength (inputViewLength);
            scene->shipList.at(j)->setPathLength (inputPathLength);
            scene->shipList.at(j)->setTime(inputTime);

            //получаем указатель на нужный лог и записываем в него все считанные данные
            QTextEdit *te = (QTextEdit*)txtStack->widget(j);
            te->append(QString("Id: %1").arg(scene->shipList.at(j)->getId()+1));

            if(scene->shipList.at(j)->getIsNew()){
                te->append(QString("Start X: %1\nStart Y: %2")
                           .arg(scene->shipList.at(j)->getStartX())
                           .arg(scene->shipList.at(j)->getStartY()));
            }

            te->append(QString("Course angle: %1 deg\nSpeed: %2\nView angle: %3 deg\nViewLength: %4\nPath length: %5 m\nTime: %6 sec\n")
                       .arg(scene->shipList.at(j)->getCourseAngle())
                       .arg(scene->shipList.at(j)->getSpeed()).arg(scene->shipList.at(j)->getViewAngle())
                       .arg(scene->shipList.at(j)->getViewLength()).arg(scene->shipList.at(j)->getPathLength()).arg(scene->shipList.at(j)->getTime()/1000.0f));
        }
        //новый блок данных
        nextBlockSize = 0;
        scene->drawBackground(painterScene,scene->sceneRect());
        scene->advance();

    }
}

//вкл/выкл область видимости
void MyClient::slotReactToToggleViewCheckBox(bool checked)
{
    for(int i = 0; i < scene->shipList.size(); i++){
        scene->shipList.at(i)->setIsViewVisible(checked);
    }
}

//вкл/выкл путь
void MyClient::slotReactToTogglePathCheckBox(bool checked){
    scene->setIsPathVisible(checked);
}

//подключиться к серверу
void MyClient::slotConnectButton(){

    //если клиент уже подключен к серверу - закрываем сокет(кнопка disconnect)
    if(isConnected){

        clearAllData();
        messageLabel->setText("Disconnected");

    }
    //если клиент еще не подключен к серверу - пытаемся подключиться(кнопка connect)
    else{
        socket->connectToHost(host,port);

        connect(socket,SIGNAL(connected()),SLOT(slotConnected()));
        connect(socket, SIGNAL(readyRead()), SLOT(slotReadyRead()));
        connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(slotError(QAbstractSocket::SocketError)));

    }
}

//соединение установлено
void MyClient::slotConnected()
{
    isConnected = true;
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
        clearAllData();
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
    for(int i = 0; i < scene->shipList.size(); i++){
        scene->shipList.at(i)->setShipSize(val);
    }
}

//изменение ширины пути
void MyClient::slotPathResize(int val){
    scene->setPathWidth (val);
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

//очистка путей
void MyClient::slotClearAllPaths()
{
    for(int i = 0; i < scene->shipList.size(); i++){
        scene->shipList.at(i)->points.clear();
    }

}

//удаление корабля с номером лога num
void MyClient::deleteShip(int num)
{
    int currentLogNumber = logNumber->text().toInt(); //показываемый в клиенте лог

    //получаем адрес лога удаляемого корабля
    QTextEdit *txt = (QTextEdit*) txtStack->widget(num-1);

    int idOfDeleted = scene->shipList.at(num-1)->getId();

    //удаляем корабль из сцены и вектора
    ShipItem *ship = scene->shipList.at(num-1);
    idOfExistingShips.remove(ship->getId());
    scene->removeItem(ship);

    scene->shipList.remove(num-1);
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

}

//удаление всех данных
void MyClient::clearAllData()
{
    socket->close();

    quint16 count = shipCounter;

    for(int i = 1; i <= count; i++){
        deleteShip(1);
    }

    connectButton->setText("Connect");
    connectButton->setEnabled(true);

    isConnected = false;

}


MyClient::~MyClient()
{
    delete scene;
}
