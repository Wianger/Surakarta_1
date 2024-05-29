#ifndef CHOOSE_H
#define CHOOSE_H

#include <QDialog>
#include <settings.h>
#include <login.h>

namespace Ui {
class Choose;
}

class Choose : public QDialog
{
    Q_OBJECT

public:
    explicit Choose(QWidget *parent = nullptr);
    ~Choose();

private slots:
    void on_Local_clicked();

    void on_Net_clicked();

private:
    Ui::Choose *ui;
    Settings *settings;
    Login *login;
};

#endif // CHOOSE_H
