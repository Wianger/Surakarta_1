#include "login.h"
#include "ui_login.h"

Login::Login(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Login)
{
    ui->setupUi(this);
}

Login::~Login()
{
    delete ui;
}


void Login::on_Confirm_clicked()
{
    if(!ui->UsernameEdit->text().isEmpty() && !ui->RoomEdit->text().isEmpty() && !ui->PortEdit->text().isEmpty() && !ui->IPEdit->text().isEmpty()){
        settings = new Settings;
        settings->Set(false);
        if(ui->PlayerEdit->text().isEmpty()){
            accept();
            settings->SetInfo(ui->UsernameEdit->text(), "", ui->RoomEdit->text(), ui->PortEdit->text(), ui->IPEdit->text());
            settings->show();
        }
        else{
            if(ui->PlayerEdit->text() != "BLACK" && ui->PlayerEdit->text() != "WHITE")
                QMessageBox::information(this, "Fail", "请检查输入");
            else{
                accept();
                settings->SetInfo(ui->UsernameEdit->text(), ui->PlayerEdit->text(), ui->RoomEdit->text(), ui->PortEdit->text(), ui->IPEdit->text());
                settings->show();
            }
        }
    }
    else
        QMessageBox::information(this, "Fail", "请检查输入");
}

