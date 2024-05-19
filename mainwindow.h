#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <surakartagame.h>
#include <QTimer>
#include <surakartaboard.h>
#include <QMessageBox>
#include "../NetworkLibrary/networksocket.h"
#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(unsigned int b = 6, unsigned int c = 10, QString player = "BLACK", QWidget *parent = nullptr);
    ~MainWindow();
    void updatePlayerInfo();
    void endGame();
    void restartGame();
    void setInfo(QString, QString, QString);
    void connected_successfully();
    void connectToServer();
    void disconnectFromServer();
    void sendMessage(OPCODE, QString, QString);
    void receiveMessage(NetworkData);

private:
    Ui::MainWindow *ui;
    SurakartaGame *game;
    QTimer *timer;
    unsigned int CountDown, restTime;
    int port = 10086;
    QString ip = "127.0.0.1";
    NetworkSocket *socket;
    QString username, room;

private slots:
    void on_move_clicked();
    void updateCountdown();
    void on_resigne_clicked();
    void on_send_msg_clicked();
};

#endif // MAINWINDOW_H
