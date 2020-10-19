#ifndef CONNECTIONDIALOG_H
#define CONNECTIONDIALOG_H

#include <QDialog>
#include <QTcpSocket>

namespace Ui {
class ConnectionDialog;
}

class ConnectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectionDialog(QWidget *parent = nullptr);
    ~ConnectionDialog();

    QTcpSocket *getSocket();

private slots:
    void on_connectButton_clicked();

    void on_cancelButton_clicked();

    void onConnected();
    void onError();

private:
    Ui::ConnectionDialog *ui;

    QTcpSocket *socket = nullptr;
};

#endif // CONNECTIONDIALOG_H
