#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <setting.h>

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);

private slots:
    void onLoginClicked();

private:
    QLineEdit *usernameEdit;
    QLineEdit *playerEdit;
    QLineEdit *roomEdit;
    QLineEdit *portEdit;
    QLineEdit *ipEdit;
    QPushButton *loginButton;
    Setting *setting;
};

#endif // LOGINDIALOG_H
