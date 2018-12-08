#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QLayout>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    drawer = new LedMatrixDrawer();
    reader = new SerialReader(new DataHandler(drawer));

    QLabel *status = new QLabel("status");
    QLabel *fps = new QLabel("led fps: 0");
    ui->statusBar->addWidget(status);
    ui->statusBar->addWidget(fps);

    reader->setDebugWidget(status);
    drawer->setFpsLabel(fps);

    init_layout();

    connect(reader->getPort(), &QIODevice::readyRead, this, &MainWindow::serialRead);
    reader->refreshEvent();

    initTimer();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initTimer()
{
    tmr = new QTimer();
    connect(tmr, SIGNAL(timeout()), this, SLOT(updateTime()));
    setTimerInterval(10);
}

void MainWindow::setTimerInterval(int val)
{
    tmr->stop();
    if (val > 0) {
        tmr->setInterval((int) ((float) 10000 / val));
        tmr->start();
    }
}

void MainWindow::updateTime()
{
    drawer->show_fps();
}

void MainWindow::init_layout() {
    QLayout *buts = drawer->initButtonMassive();
    QLayout *con_pan = init_control_panel();

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    mainLayout->addLayout(con_pan);
    mainLayout->addLayout(buts);

    QWidget *widget = new QWidget();
    widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    widget->setLayout(mainLayout);
    setCentralWidget(widget);

    MainWindow::adjustSize();
}

QLayout *MainWindow::init_control_panel()
{
    const QSize size = QSize(100, 25);

    QGridLayout *con_pan = new QGridLayout();
    con_pan->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    QPushButton *refreshButton = new QPushButton();
    refreshButton->setFixedSize(size);
    refreshButton->setText("Refresh");
    refreshButton->setToolTip("Refresh COM Ports list");
    connect(refreshButton, SIGNAL (released()), this, SLOT (refreshButtonHandler()));

    QComboBox *comboBox = new QComboBox();
    comboBox->setFixedSize(size);
    reader->setComboBox(comboBox);
    connect(comboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(comboboxItemChanged(QString)));

    QPushButton *stopButton = new QPushButton();
    stopButton->setFixedSize(size);
    stopButton->setText("Disconnect");
    stopButton->setToolTip("Interrupt the current connection");
    connect(stopButton, SIGNAL (released()), this, SLOT (stopButtonHandler()));

    QPushButton *reconnectButton = new QPushButton();
    reconnectButton->setFixedSize(size);
    reconnectButton->setText("Reconnect");
    connect(reconnectButton, SIGNAL (released()), this, SLOT (reconnectButtonHandler()));

    con_pan->addWidget(comboBox);
    con_pan->addWidget(refreshButton);
    con_pan->addWidget(stopButton);
    con_pan->addWidget(reconnectButton);

    return con_pan;
}

void MainWindow::serialRead()
{
    reader->serialRead();
}

void MainWindow::refreshButtonHandler()
{
    reader->refreshEvent();
}

void MainWindow::stopButtonHandler()
{
    reader->disconnect();
}

void MainWindow::reconnectButtonHandler()
{
    reader->reconnectEvent();
}

void MainWindow::comboboxItemChanged(const QString &arg1)
{
    reader->comboBoxValueChanged(arg1);
}
