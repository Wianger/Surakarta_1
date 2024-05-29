#include "choose.h"
#include "ui_choose.h"

Choose::Choose(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Choose)
{
    ui->setupUi(this);
}

Choose::~Choose()
{
    delete ui;
}

void Choose::on_Local_clicked()
{
    accept();
    settings = new Settings;
    settings->Set(true);
    settings->show();
}


void Choose::on_Net_clicked()
{
    accept();
    login = new Login;
    login->show();
}

