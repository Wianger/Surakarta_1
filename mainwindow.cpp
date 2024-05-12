#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <surakartaboard.h>

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
    if(SurakartaBoard::selected_num == 2)
        game->Move(SurakartaMove(SurakartaBoard::from, SurakartaBoard::to, game->game_info_->current_player_));
}

