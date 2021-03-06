#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    splineSeries = new QSplineSeries(this);
    for (int i = 0; i < 100; ++i) {
        splineSeries->append(i, 0.0);
    }

    chart = new QChart();
    chart->addSeries(splineSeries);
    chart->legend()->hide();
    chart->setTitle("Signal");
    chart->createDefaultAxes();

    ChartView *view = new ChartView(chart, this);
    view->setRenderHint(QPainter::Antialiasing);

    ui->chart->insertWidget(0, view);

    ui->disconnectButton->setEnabled(false);
    ui->startButton->setEnabled(false);
    ui->stopButton->setEnabled(false);

    connect(ui->actionColour, &QAction::triggered, this, &MainWindow::startColourDialog);
    connect(ui->actionWidth, &QAction::triggered, this, &MainWindow::startWidthDialog);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_periodSlider_sliderMoved(int position)
{
    if (!socket) {
        return;
    }

    qreal newPeriod = position / PRECISION;

    JsonParser::Message m = {.command = JsonParser::PERIOD, .param = newPeriod};
    QByteArray json = JsonParser::toJson(m);

    socket->write(json);
}

void MainWindow::on_amplitudeSlider_sliderMoved(int position)
{
    if (!socket) {
        return;
    }

    qreal newAmplitude = position / PRECISION;

    JsonParser::Message m = {.command = JsonParser::AMPLITUDE, .param = newAmplitude};
    QByteArray json = JsonParser::toJson(m);

    socket->write(json);
}

void MainWindow::on_connectButton_clicked()
{
    ConnectionDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        socket = dialog.getSocket();
        connect(socket, &QIODevice::readyRead, this, &MainWindow::commandFromServer);

        ui->connectButton->setEnabled(false);
        ui->disconnectButton->setEnabled(true);
    }
}

void MainWindow::on_disconnectButton_clicked()
{
    JsonParser::Message m = {.command = JsonParser::DISCONNECT, .param = 0.0};
    QByteArray json = JsonParser::toJson(m);

    socket->write(json);

    socket->abort();
    delete socket;
    socket = nullptr;

    ui->disconnectButton->setEnabled(false);
    ui->startButton->setEnabled(false);
    ui->connectButton->setEnabled(true);
}

void MainWindow::on_startButton_clicked()
{
    JsonParser::Message m = {.command = JsonParser::START, .param = 0.0};
    QByteArray json = JsonParser::toJson(m);

    socket->write(json);

    ui->startButton->setEnabled(false);
    ui->stopButton->setEnabled(true);
}

void MainWindow::on_stopButton_clicked()
{
    JsonParser::Message m = {.command = JsonParser::STOP, .param = 0.0};
    QByteArray json = JsonParser::toJson(m);

    socket->write(json);

    ui->stopButton->setEnabled(false);
    ui->startButton->setEnabled(true);
}

void MainWindow::commandFromServer()
{
    QByteArray json = socket->readAll();

    JsonParser::Message m = JsonParser::fromJson(json);
    switch (m.command) {
    case JsonParser::READY: {
        ui->startButton->setEnabled(true);
        break;
    }
    case JsonParser::NEXT_VAL: {
        qreal nextX = splineSeries->at(splineSeries->count() - 1).x() + 1.0;
        splineSeries->append(nextX, m.param);
        splineSeries->remove(0);

        chart->axes(Qt::Horizontal).at(0)->setRange(splineSeries->at(0).x(), nextX);
        break;
    }
    default: {}
    }
}

void MainWindow::setColour(const QColor &color)
{
    splineSeries->setColor(color);
}

void MainWindow::startColourDialog()
{
    QColorDialog *colourDialog = new QColorDialog(this);
    colourDialog->setAttribute(Qt::WA_DeleteOnClose);
    colourDialog->setModal(false);

    connect(colourDialog, &QColorDialog::currentColorChanged, this, &MainWindow::setColour);
    connect(colourDialog, &QColorDialog::colorSelected, this, &MainWindow::setColour);

    colourDialog->show();
}

void MainWindow::setWidth(int newWidth)
{
    QPen pen = splineSeries->pen();
    pen.setWidth(newWidth);
    splineSeries->setPen(pen);
}

void MainWindow::startWidthDialog()
{
    WidthDialog *widthDialog = new WidthDialog(this);
    widthDialog->setAttribute(Qt::WA_DeleteOnClose);
    widthDialog->setModal(false);

    connect(widthDialog, &WidthDialog::valChanged, this, &MainWindow::setWidth);

    widthDialog->show();
}
