#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLineEdit>
#include <QVBoxLayout>
#include <string>
#include <iostream>

QString player2str(SurakartaPlayer pl)
{
    QString player;
    if(pl == PieceColor::BLACK)
        player = "Black";
    else
        player = "White";
    return player;
}

QString endReason2String(SurakartaEndReason endReason) {
    switch (endReason) {
    case SurakartaEndReason::NONE:
        return "NONE";
    case SurakartaEndReason::STALEMATE:
        return "STALEMATE";
    case SurakartaEndReason::CHECKMATE:
        return "CHECKMATE";
    case SurakartaEndReason::TRAPPED:
        return "TRAPPED";
    case SurakartaEndReason::RESIGN:
        return "RESIGN";
    case SurakartaEndReason::TIMEOUT:
        return "TIMEOUT";
    case SurakartaEndReason:: ILLIGAL_MOVE:
        return "ILLEGAL_MOVE";
    default:
        return "UNKNOWN";
    }
}

MainWindow::MainWindow(unsigned int boardsize, unsigned int countdown, QString p, QString u,
                       QString r, int pt, unsigned int round, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , CountDown(countdown), restTime(countdown)
    , timer(new QTimer(this))
    , socket(new NetworkSocket(new QTcpSocket(this), this))
    , port(pt), username(u), room(r)
{
    ui->setupUi(this);
    ui->move->setEnabled(false);
    ui->resigne->setEnabled(false);
    ui->ai->setEnabled(false);
    game = new SurakartaGame(ui->centralwidget, boardsize, p);
    game->board_->setGeometry(0, 0, SIZE + 10, SIZE + 10);
    //game->SetPlayer(p);
    //game->StartGame();
    game->game_info_->max_no_capture_round_ = round;
    //agent = new SurakartaAgent(game->GetBoard(), game->GetGameInfo(), game->GetRuleManager());
    ui->label->setText("Current_Player : " + player2str(game->game_info_->current_player_));
    connect(timer, &QTimer::timeout, this, &MainWindow::updateCountdown);
    ui->label_2->setText("CountDown : " + QString::number(restTime) + "s");
    connect(socket->base(), &QTcpSocket::connected, this, &MainWindow::connected_successfully);
    connect(socket, &NetworkSocket::receive, this, &MainWindow::receiveMessage);
    file.setFileName("C:\\Surakarta\\Testing\\game.txt");
    bool a = file.open(QIODevice::WriteOnly);
    if(a)
        std::cout<<"FILEOPEN"<<std::endl;
}

void MainWindow::Move(SurakartaMove move)
{
    game->Move(move);
    restTime = CountDown;
    ui->label_2->setText("CountDown : " + QString::number(restTime) + "s");
    timer->start(1000);
    updatePlayerInfo();
}

void MainWindow::on_move_clicked()
{
    /*auto move = agent->CalculateMove();
    NetworkData data = NetworkData(OPCODE::MOVE_OP, QString(char('A'+move.from.x)) + QString::number(move.from.y),
                                   QString(char('A'+move.to.x)) + QString::number(move.to.y), "");
    std::cout<<(data.data1 + "->" + data.data2 + " ").toStdString()<<std::endl;
    file.write((data.data1 + "->" + data.data2 + " ").toUtf8());
    file.flush();
    sendMessage(data.op, data.data1, data.data2, "");
    Move(move);
    if(game->game_info_->IsEnd()){
        endGame(data.data1, data.data2, data.data3);
    }*/
    SurakartaMove move = SurakartaMove(SurakartaBoard::from, SurakartaBoard::to, game->game_info_->current_player_);
    NetworkData data = NetworkData(OPCODE::MOVE_OP, QString(char('A'+move.from.x)) + QString::number(move.from.y),
                                       QString(char('A'+move.to.x)) + QString::number(move.to.y), "");
    file.write((data.data1 + "->" + data.data2 + " ").toUtf8());
    file.flush();
    sendMessage(data.op, data.data1, data.data2, "");
    if(SurakartaBoard::selected_num == 2){
        Move(move);
    }
}

void MainWindow::updatePlayerInfo()
{
    ui->label->setText("Current_Player : " + player2str(game->game_info_->current_player_));
}

void MainWindow::updateCountdown()
{
    if (restTime > 0) {
        restTime--;
        ui->label_2->setText("CountDown : " + QString::number(restTime) + "s");
        update();
    } else {
        timer->stop();
    }
}

void MainWindow::endGame(QString SurakartaIllegalMoveReason, QString SurakartaEndReason, QString Winner)
{
    //game->SaveGame("game.txt");
    QString endMessage;
    endMessage = "SurakartaIllegalMoveReason: " + SurakartaIllegalMoveReason + "\n"
                 + "SurakartaEndReason: " + SurakartaEndReason + "\n"
                 + "Winner: " + Winner + "\n"
                 + "步数: " + QString::number(game->game_info_->num_round_) + "\n";
    timer->stop();

    QMessageBox::StandardButton result = QMessageBox::information(this, "游戏结束", endMessage);
    if (result == QMessageBox::Ok)
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "游戏结束", "是否再来一局", QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes)
            restartGame();
        else if(reply == QMessageBox::Cancel || reply == QMessageBox::Abort){
            sendMessage(OPCODE::LEAVE_OP, "", "", "");
            disconnect();
        }
    }
}

void MainWindow::restartGame()
{
    connected_successfully();
    game->StartGame();
    updatePlayerInfo();
    SurakartaBoard::selected_num = 0;
    restTime = CountDown;
    ui->label_2->setText("CountDown : " + QString::number(restTime) + "s");
    update();
}

void MainWindow::on_resigne_clicked()
{
    SurakartaPlayer cp = game->game_info_->current_player_;
    if(cp == game->p){
        sendMessage(OPCODE::RESIGN_OP, "", "", "");
    }
}

void MainWindow::connectToServer() {
    socket->hello(ip, port);
    this->socket->base()->waitForConnected(2000);
}

void MainWindow::connected_successfully() {
    sendMessage(OPCODE::READY_OP, username, game->player, room);
}

void MainWindow::disconnectFromServer() {
    sendMessage(OPCODE::LEAVE_OP, game->player, "", "");
    socket->bye();
}

void MainWindow::sendMessage(OPCODE s, QString u, QString message1, QString message2) {
    ui->receive_edit->append(QString::number(static_cast<int>(s)) + " " + u + " " + message1 + " " + message2);
    socket->send(NetworkData(s, u, message1, message2));
    ui->send_edit->clear();
}

void MainWindow::receiveMessage(NetworkData data) {
    ui->receive_edit->append(QString::number(static_cast<int>(data.op)) + " " + data.data1 + " " + data.data2 + " " + data.data3);
    if(data.op == OPCODE::READY_OP) {
        game->SetPlayer(data.data2);
        game->StartGame();
        agent = new SurakartaAgent(game->GetBoard(), game->GetGameInfo(), game->GetRuleManager());
        ui->move->setEnabled(true);
        ui->resigne->setEnabled(true);
        ui->ai->setEnabled(true);
        if(game->player == "BALCK"){
            timer->start(1000);
            if(is_ai){
                auto move = agent->CalculateMove();
                NetworkData data = NetworkData(OPCODE::MOVE_OP, QString(char('A'+move.from.x)) + QString::number(move.from.y),
                                               QString(char('A'+move.to.x)) + QString::number(move.to.y), "");
                file.write((data.data1 + "->" + data.data2 + " ").toUtf8());
                file.flush();
                sendMessage(data.op, data.data1, data.data2, "");
                Move(move);
            }
        }
    }
    else if(data.op == OPCODE::MOVE_OP){
        std::string t = data.data1.toStdString();
        SurakartaPosition from = SurakartaPosition((t[0] - 65), (t[1] - 48));
        t = data.data2.toStdString();
        SurakartaPosition to = SurakartaPosition((t[0] - 65), (t[1] - 48));
        SurakartaMove move = SurakartaMove(from, to, game->game_info_->current_player_);
        NetworkData data = NetworkData(OPCODE::MOVE_OP, QString(char('A'+move.from.x)) + QString::number(move.from.y),
                                       QString(char('A'+move.to.x)) + QString::number(move.to.y), "");
        file.write((data.data1 + "->" + data.data2 + " ").toUtf8());
        file.flush();
        Move(move);
        if(is_ai){
            auto move = agent->CalculateMove();
            Move(move);
        }
    }
    else if(data.op == OPCODE::END_OP){
        endGame(data.data1, data.data2, data.data3);
    }
}

void MainWindow::on_send_msg_clicked()
{
    QString message = ui->send_edit->text();
    sendMessage(OPCODE::CHAT_OP, username, message, "");
}

MainWindow::~MainWindow() {
    delete ui;
    delete game;
    delete timer;
    delete socket;
    file.close();
}


void MainWindow::on_ai_clicked()
{
    if(is_ai){
        ui->move->setEnabled(false);
        is_ai = true;
    }
    else{
        ui->move->setEnabled(true);
        is_ai = false;
    }
}


void MainWindow::on_prompt_clicked()
{
    if(SurakartaBoard::selected_num == 1){
        for (unsigned int i = 0; i < game->board_size_; ++i) {
            for (unsigned int j = 0; j < game->board_size_; ++j) {
                SurakartaIllegalMoveReason reason = game->rule_manager_->JudgeMove(SurakartaMove(SurakartaBoard::from,
                                                    SurakartaPosition(i, j), game->game_info_->current_player_));
                if(reason == SurakartaIllegalMoveReason::LEGAL_CAPTURE_MOVE || reason == SurakartaIllegalMoveReason::LEGAL_NON_CAPTURE_MOVE){
                    (*game->board_)[i][j]->SetColor(PieceColor::RED);
                    game->board_->scene->update();
                }
                else{
                    (*game->board_)[i][j]->Recover_Color();
                    game->board_->scene->update();
                }
            }
        }
    }
}

