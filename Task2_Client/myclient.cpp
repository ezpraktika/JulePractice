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

    //панель отрисовки

    scene = new QGraphicsScene;
    QGraphicsView *view = new QGraphicsView(scene);
    view->setRenderHint(QPainter::Antialiasing);
    scene->setSceneRect(0,0,700,400);

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
    connectButton->setMinimumHeight(40);

    QCheckBox *showPathCheckBox = new QCheckBox("Путь");
    QCheckBox *showViewCheckBox = new QCheckBox("Угол обзора");

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
    controlButtons->addSpacing(100);
    controlButtons->addLayout(checkBoxLayout);
    controlButtons->addSpacing(60);
    controlButtons->addLayout(labelLayout);
    controlButtons->addLayout(sliderLayout);
    controlButtons->addSpacing(80);

    //левая часть окна

    QVBoxLayout *leftPanelLayout = new QVBoxLayout;
    leftPanelLayout->addWidget(view);
    leftPanelLayout->addLayout(controlButtons);

    //панель управления логами

    QPushButton *prevButton = new QPushButton("<<");
    prevButton->setEnabled(false);

    QPushButton *nextButton = new QPushButton(">>");
    nextButton->setEnabled(false);

    QLabel *logNumber = new QLabel("1");

    QHBoxLayout *logButtonsLayout = new QHBoxLayout;
    logButtonsLayout->addWidget(prevButton);
    logButtonsLayout->addWidget(logNumber);
    logButtonsLayout->addWidget(nextButton);

    //поле логов

    txt = new QTextEdit();
    txt->setReadOnly(true);
    txt->setMinimumSize(180,425);
    txt->setMaximumWidth(165);

    //правая часть окна

    QVBoxLayout *rightPanelLayout = new QVBoxLayout();
    rightPanelLayout->addWidget(txt);
    rightPanelLayout->addLayout(logButtonsLayout);

    //интерфейс целиком

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
