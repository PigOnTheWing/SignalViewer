#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts>
#include <jsonparser.h>
#include "connectiondialog.h"

#define PRECISION 200.0

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_periodSlider_sliderMoved(int position);
    void on_amplitudeSlider_sliderMoved(int position);

    void on_connectButton_clicked();
    void on_disconnectButton_clicked();

    void on_startButton_clicked();
    void on_stopButton_clicked();

    void commandFromServer();

private:
    Ui::MainWindow *ui;

    QSplineSeries *splineSeries = nullptr;
    QChart *chart = nullptr;
    QTcpSocket *socket = nullptr;
};
#endif // MAINWINDOW_H
