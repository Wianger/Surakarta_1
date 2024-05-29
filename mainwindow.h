#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#pragma once
#include <QMainWindow>
#include <surakartagame.h>
#include <QTimer>
#include <QMessageBox>
#include "../NetworkLibrary/networksocket.h"
#include "surakartaagent.h"
#include <QFile>
#include <QStringList>
#include <limits>
#include <login.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(unsigned int b = 6, unsigned int c = 10, unsigned int round = 40, QWidget *parent = nullptr);
    ~MainWindow();
    void updatePlayerInfo();
    void endGame(QString, QString);
    void restartGame();
    void connected_successfully();
    void connectToServer();
    void disconnectFromServer();
    void sendMessage(OPCODE, QString, QString, QString);
    void receiveMessage(NetworkData);
    void Move(SurakartaMove, bool);
    void SetNetInfo(QString, QString, QString, QString, QString);
    void SetNet(bool t) { is_net = t; }
    void removeGameView();
    void play();
    void init();

private:
    Ui::MainWindow *ui;
    SurakartaGame *game;
    unsigned int restTime;
    QTimer *timer, *_play, *_ai;
    NetworkSocket *socket;
    SurakartaAgent *agent;
    Login *login;
    Settings *settings;
    int _port;
    QString _ip = "127.0.0.1", _username, _room;
    bool is_ai = false, is_net = false, file_open = false, is_appear = false, forward = true;
    QFile file;
    qsizetype step = 0, jump_step = std::numeric_limits<qsizetype>::max();
    QString record;
    QStringList move;


private slots:
    void aiMove();
    void updateCountdown();
    void onSendMsgClicked();
    void onPromptClicked();
    void canclePrompt();
    void on_ai_clicked();
    void on_resigne_clicked();
    void on_reappear_clicked();
    void on_next_clicked();
    void on_back_clicked();
    void on_jump_clicked();
    void onMoveClicked();
    void on_capture_clicked();
};

#endif // MAINWINDOW_H
