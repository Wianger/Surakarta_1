#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <surakartagame.h>
#include <QTimer>
#include <surakartaboard.h>
#include <QMessageBox>
#include "networkdata.h"
#include <networksocket.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void updatePlayerInfo();
    void endGame();
    void restartGame();

private:
    Ui::MainWindow *ui;
    SurakartaGame *game;
    QTimer *timer;
    unsigned int CountDown = 10, restTime = CountDown;
    int port = 10086;
    QString ip = "127.0.0.1";
    NetworkSocket *socket;

private slots:
    void on_pushButton_clicked();
    void updateCountdown();
    void on_pushButton_2_clicked();
};

#endif // MAINWINDOW_H
