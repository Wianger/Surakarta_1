#ifndef SETTING_H
#define SETTING_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include "mainwindow.h"

class Setting : public QDialog
{
    Q_OBJECT
public:
    Setting(QWidget *parent = nullptr);
    void setInfo(QString, QString, QString, QString);

private:
    QLineEdit *boardsizeEdit;
    QLineEdit *countdownEdit;
    QLineEdit *roundEdit;
    QPushButton *confirmButton;
    QString username, player, room, port;
    MainWindow *w;

private slots:
    void onconfirmClicked();
};

#endif // SETTING_H
