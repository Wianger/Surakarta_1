#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class Settings;
}

class Settings : public QDialog
{
    Q_OBJECT

public:
    Ui::Settings *ui;
    explicit Settings(QWidget *parent = nullptr);
    ~Settings();
    void Set(bool t) { is_local = t; }
    void SetInfo(QString, QString, QString, QString, QString);
    void Strart(bool t) { is_start = t; }
    unsigned int boardsize, countdown, round;
    QString _username, _player, _room, _port, _ip;
private slots:
    void on_Confirm_clicked();

private:
    bool is_local, is_start = true;

};

#endif // SETTINGS_H
