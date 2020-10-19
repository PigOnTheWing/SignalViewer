#include "connectiondialog.h"
#include "ui_connectiondialog.h"

ConnectionDialog::ConnectionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectionDialog)
{
    ui->setupUi(this);
}

ConnectionDialog::~ConnectionDialog()
{
    delete ui;
}

void ConnectionDialog::on_connectButton_clicked()
{
    socket = new QTcpSocket();
    connect(socket, &QAbstractSocket::connected, this, &ConnectionDialog::onConnected);
    connect(socket, &QAbstractSocket::errorOccurred, this, &ConnectionDialog::onError);

    socket->connectToHost(ui->hostEdit->text(), ui->portEdit->text().toInt());
}

void ConnectionDialog::on_cancelButton_clicked()
{
    QDialog::reject();
}

QTcpSocket *ConnectionDialog::getSocket()
{
    return socket;
}

void ConnectionDialog::onConnected()
{
    QDialog::accept();
}

void ConnectionDialog::onError()
{
    QDialog::reject();
}
