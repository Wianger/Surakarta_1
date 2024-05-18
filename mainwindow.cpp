#include "mainwindow.h"
#include "ui_mainwindow.h"

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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    game = new SurakartaGame(ui->centralwidget);
    //animation = new QPropertyAnimation;
    game->board_->setGeometry(0, 0, SIZE + 10, SIZE + 10);
    game->StartGame();
    ui->label->setText("Current_Player : " + player2str(game->game_info_->current_player_));
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateCountdown);
    timer->start(1000);
    ui->label_2->setText("CountDown : " + QString::number(restTime) + "s");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    if(SurakartaBoard::selected_num == 2)
        game->Move(SurakartaMove(SurakartaBoard::from, SurakartaBoard::to, game->game_info_->current_player_));
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
    game->StartGame();
    updatePlayerInfo();
    SurakartaBoard::selected_num = 0;
    restTime = CountDown;
    timer->start(1000);
    update();
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


void MainWindow::on_pushButton_2_clicked()
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
    endGame();
}

