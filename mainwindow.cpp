#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <surakartaboard.h>
#include <cmath>
#include <iostream>

double Distance(QPointF p1, QPointF p2)
{
    return std::pow(p1.x() - p2.x(), 2) + std::pow(p1.y() - p2.y(), 2);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    game = new SurakartaGame(ui->centralwidget);
    connect(game->board_.get(), SIGNAL(mouseClicked(QPointF)), this, SLOT(on_mouseClicked(QPointF)));
    animation = new QPropertyAnimation;
    game->board_->setGeometry(0, 0, SIZE + 10, SIZE + 10);
    game->StartGame();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    std::cout<<"from: " <<SurakartaBoard::from.x<<" "<<SurakartaBoard::from.y<<std::endl;
    std::cout<<"to: " <<SurakartaBoard::to.x<<" "<<SurakartaBoard::to.y<<std::endl;
    std::cout<<game->game_info_->current_player_<<std::endl;
    if(SurakartaBoard::selected_num == 2)
        game->Move(SurakartaMove(SurakartaBoard::from, SurakartaBoard::to, game->game_info_->current_player_));
}

