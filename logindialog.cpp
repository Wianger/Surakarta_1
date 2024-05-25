#include "logindialog.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QMessageBox>

LoginDialog::LoginDialog(QWidget *parent) : QDialog(parent)
{
    QLabel *usernameLabel = new QLabel("Username:", this);
    usernameEdit = new QLineEdit(this);
    usernameEdit->setText("Team7");
    QLabel *playerLabel = new QLabel("Player:", this);
    playerEdit = new QLineEdit(this);
    playerEdit->setText("BLACK");
    QLabel *roomLabel = new QLabel("Room:", this);
    roomEdit = new QLineEdit(this);
    roomEdit->setText("1");
    QLabel *portLabel = new QLabel("Port:", this);
    portEdit = new QLineEdit(this);
    portEdit->setText("10086");
    loginButton = new QPushButton("Login", this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(usernameLabel);
    layout->addWidget(usernameEdit);
    layout->addWidget(playerLabel);
    layout->addWidget(playerEdit);
    layout->addWidget(roomLabel);
    layout->addWidget(roomEdit);
    layout->addWidget(portLabel);
    layout->addWidget(portEdit);
    layout->addWidget(loginButton);
    setLayout(layout);
    connect(loginButton, &QPushButton::clicked, this, &LoginDialog::onLoginClicked);
}

void LoginDialog::onLoginClicked()
{
    if(!usernameEdit->text().isEmpty() && !roomEdit->text().isEmpty() && !portEdit->text().isEmpty()){
        setting = new Setting;
        QString player = playerEdit->text();;
        if(playerEdit->text().isEmpty()){
            player = "";
            accept();
            setting->setInfo(usernameEdit->text(), player, roomEdit->text(), portEdit->text());
            setting->show();
        }
        else{
            if(player != "BLACK" && player != "WHITE")
                QMessageBox::information(this, "Fail", "请检查输入");
            else{
                accept();
                setting->setInfo(usernameEdit->text(), player, roomEdit->text(), portEdit->text());
                setting->show();
            }
        }
    }
    else
        QMessageBox::information(this, "Fail", "请检查输入");
}
