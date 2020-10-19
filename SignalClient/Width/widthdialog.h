#ifndef WIDTHDIALOG_H
#define WIDTHDIALOG_H

#include <QDialog>

namespace Ui {
class WidthDialog;
}

class WidthDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WidthDialog(QWidget *parent = nullptr);
    ~WidthDialog();

signals:
    void valChanged(int newVal);

private slots:
    void on_spinBox_valueChanged(int arg1);

private:
    Ui::WidthDialog *ui;
};

#endif // WIDTHDIALOG_H
