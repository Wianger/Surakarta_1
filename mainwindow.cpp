#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>
#include <iostream>

MainWindow::MainWindow(unsigned int boardsize, unsigned int countdown, unsigned int round, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , restTime(countdown)
    , timer(new QTimer(this)), _play(new QTimer(this)), _ai(new QTimer)
    , socket(new NetworkSocket(new QTcpSocket(this), this))
{
    BOARDSIZE = boardsize;
    MAX_NO_CAPTURE_ROUND = round;
    CountDown = countdown;
    ui->setupUi(this);
    game = new SurakartaGame(ui->centralwidget);
    game->board_->setGeometry(0, 0, SIZE + 10, SIZE + 10);
    game->StartGame();
    agent = new SurakartaAgent(game->GetBoard(), game->GetGameInfo(), game->GetRuleManager());
    ui->label->setText("Current_Player : " + player2str(game->game_info_->current_player_));
    connect(timer, &QTimer::timeout, this, &MainWindow::updateCountdown);
    connect(_play, &QTimer::timeout, this, &MainWindow::play);
    connect(_ai, &QTimer::timeout, this, &MainWindow::aiMove);
    ui->label_2->setText("CountDown : " + QString::number(restTime) + "s");
    connect(socket->base(), &QTcpSocket::connected, this, &MainWindow::connected_successfully);
    connect(socket, &NetworkSocket::receive, this, &MainWindow::receiveMessage);
    connect(game->board_.get(), SIGNAL(promt()), this, SLOT(onPromptClicked()));
    connect(game->board_.get(), SIGNAL(cancle()), this, SLOT(canclePrompt()));
    connect(game->board_.get(), SIGNAL(move()), this, SLOT(onMoveClicked()));
    file.setFileName("C:\\Surakarta\\Testing\\game.txt");
    if(!is_net)
        timer->start(1000);
}

void MainWindow::Move(SurakartaMove move, bool is_ai)
{
    if(!file_open){
        file_open = true;
        file.open(QIODevice::WriteOnly);
    }
    NetworkData data = NetworkData(OPCODE::MOVE_OP, QString(char('A'+move.from.x)) + QString::number(move.from.y),
                                   QString(char('A'+move.to.x)) + QString::number(move.to.y), "");
    file.write((data.data1 + "-" + data.data2 + " ").toUtf8());
    file.flush();
    if(is_net)
        sendMessage(data.op, data.data1, data.data2, "");
    //timer->stop();
    game->Move(move, is_ai);
    restTime = CountDown;
    ui->label_2->setText("CountDown : " + QString::number(restTime) + "s");
    timer->start(1000);
    updatePlayerInfo();
}

void MainWindow::SetNetInfo(QString username, QString player, QString room, QString port, QString ip)
{
    _username = username;
    _room = room;
    _port = port.toInt();
    game->player = player;
    _ip = ip;
}

void MainWindow::aiMove()
{
    auto move = agent->CalculateMove();
    Move(move, true);
    if(!is_net && game->game_info_->IsEnd()){
        file.write((endReason2String(game->game_info_->end_reason_) + "#").toUtf8());
        endGame("", "");
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
        if(!is_net){
            endGame(endReason2String(SurakartaEndReason::TIMEOUT), player2str(ReverseColor(game->game_info_->current_player_)));
        }
    }
}

void MainWindow::endGame(QString SurakartaEndReason, QString Winner)
{
    is_ai = false;
    _ai->stop();
    QString endMessage;
    endMessage = "SurakartaEndReason: " + SurakartaEndReason + "\n"
                 + "Winner: " + Winner + "\n"
                 + "步数: " + QString::number(game->game_info_->num_round_) + "\n"
                 + "净胜子数： " + QString::number(game->rule_manager_->winnum) + "\n";
    timer->stop();
    file.close();
    move.clear();
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
    game->board_->setParent(nullptr);
    delete game;
    if(is_net){
        login = new Login;
    }
    else{
        settings = new Settings;
        settings->Strart(false);
    }
    if((!is_net && settings->exec()) || (is_net && login->exec())){
        if(!is_net){
            BOARDSIZE = settings->boardsize;
            CountDown = settings->countdown;
            MAX_NO_CAPTURE_ROUND = settings->round;
        }
        else{
            SetNetInfo(settings->_username, settings->_player, settings->_room, settings->_port, settings->_ip);
            BOARDSIZE = settings->boardsize;
            CountDown = settings->countdown;
            MAX_NO_CAPTURE_ROUND = settings->round;
        }
        ui->setupUi(this);
        game = new SurakartaGame(ui->centralwidget);
        game->board_->setGeometry(0, 0, SIZE + 10, SIZE + 10);
        game->StartGame();
        game->board_->scene->update();
        agent = new SurakartaAgent(game->GetBoard(), game->GetGameInfo(), game->GetRuleManager());
        connect(ui->resigne, &QPushButton::clicked, this, &MainWindow::on_resigne_clicked);
        connect(ui->ai, &QPushButton::clicked, this, &MainWindow::on_ai_clicked);
        connect(ui->next, &QPushButton::clicked, this, &MainWindow::on_next_clicked);
        connect(ui->back, &QPushButton::clicked, this, &MainWindow::on_back_clicked);
        connect(ui->jump, &QPushButton::clicked, this, &MainWindow::on_jump_clicked);
        connect(game->board_.get(), SIGNAL(promt()), this, SLOT(onPromptClicked()));
        connect(game->board_.get(), SIGNAL(cancle()), this, SLOT(canclePrompt()));
        connect(game->board_.get(), SIGNAL(move()), this, SLOT(onMoveClicked()));
        connect(ui->capture, &QPushButton::clicked, this, &MainWindow::on_capture_clicked);
        updatePlayerInfo();
        SurakartaBoard::selected_num = 0;
        restTime = CountDown;
        ui->label_2->setText("CountDown : " + QString::number(restTime) + "s");
        if(is_net)
            connected_successfully();
        else
            timer->start(1000);
        update();
    }
}

void MainWindow::connectToServer() {
    socket->hello(_ip, _port);
    this->socket->base()->waitForConnected(2000);
}

void MainWindow::connected_successfully() {
    sendMessage(OPCODE::READY_OP, _username, game->player, _room);
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
        if(game->player == "BLACK"){
            timer->start(1000);
            if(is_ai){
                auto move = agent->CalculateMove();
                Move(move, true);
            }
        }
    }
    else if(data.op == OPCODE::MOVE_OP){
        std::string t = data.data1.toStdString();
        SurakartaPosition from = SurakartaPosition((t[0] - 65), (t[1] - 48));
        t = data.data2.toStdString();
        SurakartaPosition to = SurakartaPosition((t[0] - 65), (t[1] - 48));
        SurakartaMove move = SurakartaMove(from, to, game->game_info_->current_player_);
        Move(move, false);
        if(is_ai){
            auto move = agent->CalculateMove();
            Move(move, true);
        }
    }
    else if(data.op == OPCODE::END_OP){
        file.write((data.data2 + "#").toUtf8());
        endGame(data.data2, data.data3);
    }
    else if(data.op == OPCODE::REJECT_OP)
        endGame("", "");
}

void MainWindow::onSendMsgClicked()
{
    QString message = ui->send_edit->text();
    sendMessage(OPCODE::CHAT_OP,  _username, message, "");
}

MainWindow::~MainWindow() {
    delete ui;
    delete game;
    delete timer;
    delete socket;
    file.close();
}

void MainWindow::onPromptClicked()
{
    if(SurakartaBoard::selected_num == 1){
        for (unsigned int i = 0; i < BOARDSIZE; ++i) {
            for (unsigned int j = 0; j < BOARDSIZE; ++j) {
                SurakartaIllegalMoveReason reason = game->rule_manager_->JudgeMove(SurakartaMove(SurakartaBoard::from,
                                                                        SurakartaPosition(i, j), game->game_info_->current_player_));
                if(reason == SurakartaIllegalMoveReason::LEGAL_CAPTURE_MOVE || reason == SurakartaIllegalMoveReason::LEGAL_NON_CAPTURE_MOVE){
                    (*game->board_)[i][j]->SetColor(PieceColor::RED);
                    game->board_->scene->update();
                }
            }
        }
    }
}

void MainWindow::canclePrompt()
{
    game->Recover_Color();
    game->board_->scene->update();
}

void MainWindow::play()
{
    if(forward && step < jump_step){
        on_next_clicked();
    }
    else if(!forward && step > jump_step){
        on_back_clicked();
    }
    else{
        jump_step = std::numeric_limits<qsizetype>::max();
        forward = true;
        _play->stop();
    }
}

void MainWindow::on_ai_clicked()
{
    if(is_ai){
        is_ai = false;
        ui->ai->setText("AI托管");
        _ai->stop();
    }
    else{
        is_ai = true;
        ui->ai->setText("取消托管");
        if(!is_net){
            _ai->start(1000);
        }
    }
}

void MainWindow::on_resigne_clicked()
{
    sendMessage(OPCODE::RESIGN_OP, "", "", "");
    if(!is_net){
        endGame(endReason2String(SurakartaEndReason::RESIGN), player2str(ReverseColor(game->game_info_->current_player_)));
    }
}


void MainWindow::on_reappear_clicked()
{
    if(!is_appear){
        ui->reappear->setText("暂停");
        is_appear = true;
        _play->start(1000);
    }
    else{
        ui->reappear->setText("播放");
        is_appear = false;
        _play->stop();
    }
}

void MainWindow::on_next_clicked()
{
    std::cout<<step<<"<"<<move.size()<<std::endl;
    if(step < move.size() - 1 || step == 0){
        if(!file_open){
            file.open(QIODevice::ReadOnly);
            record = file.readAll();
            move = record.split(" ");
            file_open = true;
        }
        std::cout<<"step"<<step<<" ";
        std::string t = move[step].toStdString();
        SurakartaPlayer player;
        SurakartaPosition from = SurakartaPosition((t[0] - 65), (t[1] - 48));
        SurakartaPosition to = SurakartaPosition((t[3] - 65), (t[4] - 48));
        std::cout<<t[0]<<t[1]<<" "<<t[3]<<t[4]<<std::endl;
        if(step % 2)
            player = SurakartaPlayer::WHITE;
        else
            player = SurakartaPlayer::BLACK;
        SurakartaMove m = SurakartaMove(from, to, player);
        Move(m, true);
        timer->stop();
        step++;
    }
    else{
        step = 0;
        endGame("", "");
        file_open = false;
    }
}

void MainWindow::on_back_clicked()
{
    step--;
    std::cout<<"step"<<step<<" ";
    std::string t = move[step].toStdString();
    SurakartaPlayer player;
    SurakartaPosition to = SurakartaPosition((t[0] - 65), (t[1] - 48));
    SurakartaPosition from = SurakartaPosition((t[3] - 65), (t[4] - 48));
    std::cout<<t[0]<<t[1]<<" "<<t[3]<<t[4]<<std::endl;
    if(step % 2)
        player = SurakartaPlayer::WHITE;
    else
        player = SurakartaPlayer::BLACK;
    game->game_info_->current_player_ = player;
    SurakartaMove m = SurakartaMove(from, to, player);
    SurakartaIllegalMoveReason reason = game->rule_manager_->JudgeMove(m);
    std::cout<<reason<<std::endl;
    std::swap((*game->board_)[m.to.x][m.to.y], (*game->board_)[m.from.x][m.from.y]);
    (*game->board_)[m.to.x][m.to.y]->SetPosition(m.to);
    (*game->board_)[m.from.x][m.from.y]->SetPosition(m.from);
    if(reason == SurakartaIllegalMoveReason::ILLIGAL_NON_CAPTURE_MOVE){
        std::cout<<"ENTER"<<std::endl;
        (*game->board_)[m.from.x][m.from.y]->SetColor(ReverseColor(player));
        (*game->board_)[m.from.x][m.from.y]->setFixedColor(ReverseColor(player));
    }
    game->board_->scene->update();
}

void MainWindow::on_jump_clicked()
{
    jump_step = ui->lineEdit->text().toInt();
    if(jump_step - 1 >= step)
        forward = true;
    else
        forward = false;
    _play->start(1000);
}

void MainWindow::onMoveClicked()
{
    SurakartaMove move = SurakartaMove(SurakartaBoard::from, SurakartaBoard::to, game->game_info_->current_player_);
    if(SurakartaBoard::selected_num == 2){
        Move(move, false);
    }
    if(!is_net && game->game_info_->IsEnd()){
        file.write((endReason2String(game->game_info_->end_reason_) + "#").toUtf8());
        endGame("", "");
    }
}

void MainWindow::on_capture_clicked()
{
    if(game->is_captured){
        game->is_captured = false;
        game->timeline->stop();
        SurakartaPosition from = SurakartaBoard::from;
        SurakartaPosition to = SurakartaBoard::to;
        PieceColor color = (*game->board_)[from.x][from.y]->GetFixColor();
        (*game->board_)[from.x][from.y] = std::make_shared<SurakartaPiece>(from, PieceColor::NONE);
        game->board_->scene->addItem((*game->board_)[from.x][from.y].get());
        (*game->board_)[to.x][to.y]->SetColor(color);
        (*game->board_)[to.x][to.y]->setFixedColor(color);
        game->board_->scene->update();
    }
}

