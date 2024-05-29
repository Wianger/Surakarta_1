#include "settings.h"
#include "ui_settings.h"
#include "mainwindow.h"
#include <iostream>

Settings::Settings(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Settings)
{
    ui->setupUi(this);
}

Settings::~Settings()
{
    delete ui;
}

void Settings::SetInfo(QString username, QString player, QString room, QString port, QString ip)
{
    _username = username;
    _player = player;
    _room = room;
    _port = port;
    _ip =ip;
}

void Settings::on_Confirm_clicked()
{
    if(ui->BoardsizeEdit->text().isEmpty() || ui->CountdownEdit->text().isEmpty() || ui->MaxroundEdit->text().isEmpty())
        QMessageBox::information(this, "Fail", "请检查输入");
    else{
        boardsize = ui->BoardsizeEdit->text().toInt(),
        countdown = ui->CountdownEdit->text().toInt(),
        round = ui->MaxroundEdit->text().toInt();
        if(boardsize % 2 != 0)
            QMessageBox::information(this, "Fail", "请检查棋盘大小");
        else{
            accept();
            if(is_start){
                MainWindow *w = new MainWindow(boardsize, countdown, round);
                if(!is_local){
                    std::cout<<"11111111111111"<<std::endl;
                    w->SetNetInfo(_username, _player, _room, _port, _ip);
                    w->SetNet(true);
                    w->connectToServer();
                }
                w->show();
            }
        }
    }
}

