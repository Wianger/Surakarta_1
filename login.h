#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <settings.h>

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

private slots:
    void on_Confirm_clicked();

private:
    Ui::Login *ui;
    Settings *settings;
};

#endif // LOGIN_H
