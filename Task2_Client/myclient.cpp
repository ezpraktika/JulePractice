#include "myclient.h"

MyClient::MyClient(const QString& host, int port, QWidget *parent) :
    QWidget(parent),
    nextBlockSize(0)
{
    screen = QApplication::desktop()->screenGeometry();

    socket=new QTcpSocket(this);
    socket->connectToHost(host,port);

    connect(socket,SIGNAL(connected()),SLOT(slotConnected()));
    connect(socket, SIGNAL(readyRead()), SLOT(slotReadyRead()));
    /*connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),
    this, SLOT(slotError(QAbstractSocket::SocketError))
    );*/
    createGui();
}

void MyClient::createGui(){

    //панель отрисовки

    scene = new QGraphicsScene;
    QGraphicsView *view = new QGraphicsView(scene);

    view->setRenderHint(QPainter::Antialiasing);
    view->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    scene->setSceneRect(0,0,screen.width()*PERCENT_OF_SCREEN,screen.height()*PERCENT_OF_SCREEN);

    QPen pen = QPen(Qt::red);
    QLineF topLine (scene->sceneRect().topLeft(),scene->sceneRect().topRight());
    QLineF rightLine (scene->sceneRect().topRight(),scene->sceneRect().bottomRight());
    QLineF bottomLine (scene->sceneRect().bottomLeft(),scene->sceneRect().bottomRight());
    QLineF leftLine (scene->sceneRect().topLeft(),scene->sceneRect().bottomLeft());

    scene->addLine(topLine,pen);
    scene->addLine(rightLine,pen);
    scene->addLine(bottomLine,pen);
    scene->addLine(leftLine,pen);

    //панель управления отрисовкой

    QPushButton *connectButton = new QPushButton("Connect");

    QCheckBox *showPathCheckBox = new QCheckBox("Путь");
    QCheckBox *showViewCheckBox = new QCheckBox("Угол обзора");
    connect(showViewCheckBox,SIGNAL(toggled(bool)),this,SLOT(slotReactToToggleViewCheckBox(bool)));

    QLabel *shipSizeLabel = new QLabel("Размер корабля");
    QLabel *pathSizeLabel = new QLabel("Ширина пути");

    QSlider *shipSizeSlider = new QSlider(Qt::Horizontal);
    shipSizeSlider->setMaximumWidth(120);
    QSlider *pathSizeSlider = new QSlider(Qt::Horizontal);
    pathSizeSlider->setMaximumWidth(120);

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
    controlButtons->addWidget(connectButton);
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

        quint16 inputIsNew, inputID;
        //qreal inputStartX, inputStartY;

        in >> inputID >> inputIsNew;

        if(inputIsNew){
            ShipItem *ship = new ShipItem;
            shipList.append(ship);
            scene->addItem(ship);

            shipCounter++;

            in >> shipList.at(inputID)->startX
               >> shipList.at(inputID)->startY;
        }

        shipList.at(inputID)->id = inputID;
        shipList.at(inputID)->isNew = inputIsNew;

        in >> shipList.at(inputID)->courseAngle
           >> shipList.at(inputID)->speed
           >> shipList.at(inputID)->viewAngle
           >> shipList.at(inputID)->viewLength;
        //time and path


        if(shipCounter-1){
            QTextEdit *txt = new QTextEdit;
            txt->setReadOnly(true);
            txt->append("New Ship Created");
            txtStack->addWidget(txt);
        }

        QTextEdit *te = (QTextEdit*)txtStack->widget(inputID);
        te->append(QString("Id: %1").arg(shipList.at(inputID)->id));

        if(shipList.at(inputID)->isNew){
            te->append(QString("Start X: %1\nStart Y: %2")
                       .arg(shipList.at(inputID)->startX)
                       .arg(shipList.at(inputID)->startY));
        }

        te->append(QString("Course angle: %1\nSpeed: %2\nView angle: %3\nViewLength: %4\nPath length: %5\nTime: %6\n")
                       .arg(shipList.at(inputID)->courseAngle)
                       .arg(shipList.at(inputID)->speed).arg(shipList.at(inputID)->viewAngle)
                       .arg(shipList.at(inputID)->viewLength).arg("later").arg("later"));

        nextBlockSize = 0;
    }
    scene->advance();
}

void MyClient::slotReactToToggleViewCheckBox(bool checked)
{
   /* showView = checked;*/
    //как то передавать сцене?
}

void MyClient::slotConnected()
{
    qDebug()<<"nnice connct";
   // txt->append("Received the connected() signal");
}

MyClient::~MyClient()
{

}
