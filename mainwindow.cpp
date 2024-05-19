#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLineEdit>
#include <QVBoxLayout>

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

MainWindow::MainWindow(unsigned int boardsize, unsigned int countdown, QString p, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    game = new SurakartaGame(ui->centralwidget, boardsize, p);
    game->board_->setGeometry(0, 0, SIZE + 10, SIZE + 10);
    game->StartGame();
    ui->label->setText("Current_Player : " + player2str(game->game_info_->current_player_));
    restTime = countdown, CountDown = countdown;
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateCountdown);
    //timer->start(1000);
    ui->label_2->setText("CountDown : " + QString::number(restTime) + "s");
    socket = new NetworkSocket(new QTcpSocket(this), this);
    connect(socket->base(), &QTcpSocket::connected, this, &MainWindow::connected_successfully);  // connected 是客户端连接成功后发出的信号
    connect(socket, &NetworkSocket::receive, this, &MainWindow::receiveMessage);
}

void MainWindow::on_move_clicked()
{
    SurakartaMove move = SurakartaMove(SurakartaBoard::from, SurakartaBoard::to, game->game_info_->current_player_);
    if(SurakartaBoard::selected_num == 2){
        game->Move(move);
        sendMessage(OPCODE::MOVE_OP, QString(char('A'+move.from.x)) + QString::number(move.from.y), QString(char('A'+move.to.x)) + QString::number(move.to.y));
        if(game->game_info_->IsEnd())
            endGame();
        else{
            restTime = CountDown;
            //timer->start(1000);
            updatePlayerInfo();
        }
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
        update(); // 每秒更新界面
    } else {
        timer->stop();
        game->game_info_->end_reason_ = SurakartaEndReason::TIMEOUT;
        endGame();
    }
}

void MainWindow::endGame()
{
    game->SaveGame("game.txt");
    QString endMessage;
    if (game->game_info_->end_reason_ == SurakartaEndReason::CHECKMATE) {
        endMessage = "玩家 " + player2str(game->game_info_->winner_) + " 获胜!";
    } else if (game->game_info_->end_reason_ == SurakartaEndReason::TIMEOUT) {
        endMessage = "玩家 " + player2str(game->game_info_->current_player_) + " 超时认输!" + "\n" + "Winner : " + player2str(game->game_info_->winner_);
    } else {
        endMessage = "游戏结束，失败原因 : " + endReason2String(game->game_info_->end_reason_) + "\n" + "Winner : " + player2str(game->game_info_->winner_);
    }

    timer->stop();

    QMessageBox::StandardButton result = QMessageBox::information(this, "游戏结束", endMessage);
    if (result == QMessageBox::Ok)
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "游戏结束", "是否再来一局", QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            restartGame();
        }
    }
}

void MainWindow::restartGame()
{
    connectToServer();
    game->StartGame();
    updatePlayerInfo();
    SurakartaBoard::selected_num = 0;
    restTime = CountDown;
    //timer->start(1000);
    connected_successfully();
    update();
}

void MainWindow::setInfo(QString u, QString r, QString po)
{
    username = u;
    room = r;
    port = po.toInt();
}


/*void MainWindow::on_pushButton_2_clicked()
{
    if(game->is_captured){
        (*game->board_)[SurakartaBoard::to.x][SurakartaBoard::to.y]->SetColor(PieceColor::NONE);
        (*game->board_)[SurakartaBoard::to.x][SurakartaBoard::to.y]->setFixedColor(PieceColor::NONE);
        (*game->board_)[SurakartaBoard::to.x][SurakartaBoard::to.y]->SetPosition(SurakartaBoard::from);
        (*game->board_)[SurakartaBoard::from.x][SurakartaBoard::from.y]->SetPosition(SurakartaBoard::to);
        std::swap((*game->board_)[SurakartaBoard::to.x][SurakartaBoard::to.y], (*game->board_)[SurakartaBoard::from.x][SurakartaBoard::from.y]);
        std::cout<<" "<<(*game->board_)[SurakartaBoard::to.x][SurakartaBoard::to.y]->GetColor()<<std::endl;
        std::cout<<" "<<(*game->board_)[SurakartaBoard::from.x][SurakartaBoard::from.y]->GetPosition().x<<" "<<(*game->board_)[SurakartaBoard::from.x][SurakartaBoard::from.y]->GetPosition().y<<std::endl;
        std::cout<<" "<<(*game->board_)[SurakartaBoard::to.x][SurakartaBoard::to.y]->GetPosition().x<<" "<<(*game->board_)[SurakartaBoard::to.x][SurakartaBoard::to.y]->GetPosition().y<<std::endl;
        game->is_captured = false;
        game->board_->scene->update();
    }
}*/


void MainWindow::on_resigne_clicked()
{
    SurakartaPlayer current_player = game->game_info_->current_player_;
    if (current_player == PieceColor::BLACK)
        QMessageBox::information(this, "认输", "黑棋认输");
    else if (current_player == PieceColor::WHITE)
        QMessageBox::information(this, "认输", "白棋认输");

    game->game_info_->end_reason_=SurakartaEndReason::RESIGN;
    SurakartaPlayer cp = game->game_info_->current_player_;
    if(cp == PieceColor::BLACK)
        game->game_info_->winner_ = PieceColor::WHITE;
    else
        game->game_info_->winner_ = PieceColor::BLACK;
    sendMessage(OPCODE::RESIGN_OP, "", "");
    endGame();

}

void MainWindow::connectToServer() {
    socket->hello(ip, port);                       // 连接服务器 ip:port
    this->socket->base()->waitForConnected(2000);  // 等待连接，2s 后超时
}

void MainWindow::connected_successfully() {
    // 连接成功后，设置界面的状态
    socket->send(NetworkData(OPCODE::READY_OP, username, game->player, room));
    // 这个程序中，连接成功后，发送一个消息给服务器，告诉服务器我已经准备好了,这不是网络中必须的操作，但是在游戏中，我们可能会规定这样的行为
}

void MainWindow::disconnectFromServer() {
    socket->send(NetworkData(OPCODE::LEAVE_OP, "", "", ""));
    socket->bye();
}

void MainWindow::sendMessage(OPCODE op, QString m1, QString m2) {
    socket->send(NetworkData(op, m1, m2, "")); // 发送消息给服务端，是不是很简单
    ui->send_edit->clear();
}

void MainWindow::receiveMessage(NetworkData data) {
    ui->receive_edit->append(data.data1); // data 是收到的消息，我们显示出来
    ui->receive_edit->append(data.data2);
    ui->receive_edit->append(data.data3);
}

MainWindow::~MainWindow() {
    delete ui;
    delete socket;
}

void MainWindow::on_send_msg_clicked()
{
    QString message = ui->send_edit->text();
    sendMessage(OPCODE::CHAT_OP, username, message);
    ui->receive_edit->append(message);
}

