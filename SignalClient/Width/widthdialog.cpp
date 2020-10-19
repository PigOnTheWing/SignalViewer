#include "widthdialog.h"
#include "ui_widthdialog.h"

WidthDialog::WidthDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WidthDialog)
{
    ui->setupUi(this);
}

WidthDialog::~WidthDialog()
{
    delete ui;
}

void WidthDialog::on_spinBox_valueChanged(int arg1)
{
    emit valChanged(arg1);
}
