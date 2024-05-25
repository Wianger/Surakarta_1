#include "surakartagame.h"
#include <fstream>
#include <iostream>

qreal findNearearPercentage(const QPainterPath& path, const QPointF& point)
{
    qreal minDistance = std::numeric_limits<qreal>::max();
    qreal nearestPercentage = 0.0;
    for (int t = 0; t <= 1; t += 0.01) {
        QPointF pathPoint = path.pointAtPercent(t);
        qreal distance = QLineF(pathPoint, point).length();
        if(distance < minDistance){
            minDistance = distance;
            nearestPercentage = t;
        }
    }
    return nearestPercentage;
}

void SurakartaGame::StartGame(std::string file_name) {
    if (file_name.empty()) {
        QPen pen;
        pen.setWidth(2);
        QBrush brush;
        brush.setColor(Qt::white);
        QRectF rect(GAP_SIZE, GAP_SIZE, SIZE - GAP_SIZE * 2, SIZE - GAP_SIZE * 2);
        board_->scene->addRect(rect, pen, brush);
        std::cout<<board_size_<<std::endl;
        for (unsigned int var = 1; var < board_size_ / 2; ++var) {
            QPainterPath path;
            path.moveTo(QPointF(GAP_SIZE + board_->square_size * var, GAP_SIZE));
            path.lineTo(QPointF(GAP_SIZE + board_->square_size * var, SIZE - GAP_SIZE));
            rect.setRect(GAP_SIZE - board_->square_size * var, SIZE - GAP_SIZE - board_->square_size * var, board_->square_size * var * 2, board_->square_size * var * 2);
            path.arcTo(rect, 0, -270);
            path.arcMoveTo(rect, 90);
            path.lineTo(SIZE - GAP_SIZE, SIZE - GAP_SIZE - board_->square_size * var);
            rect.setRect(SIZE - GAP_SIZE - board_->square_size * var, SIZE - GAP_SIZE - board_->square_size * var, board_->square_size * var * 2, board_->square_size * var * 2);
            path.arcTo(rect, 90, -270);
            path.arcMoveTo(rect, 180);
            path.lineTo(SIZE - GAP_SIZE - board_->square_size * var, GAP_SIZE);
            rect.setRect(SIZE - GAP_SIZE - board_->square_size * var, GAP_SIZE - board_->square_size * var, board_->square_size * var * 2, board_->square_size * var * 2);
            path.arcTo(rect, 180, -270);
            path.arcMoveTo(rect, 270);
            path.lineTo(GAP_SIZE, GAP_SIZE + board_->square_size * var);
            rect.setRect(GAP_SIZE - board_->square_size * var, GAP_SIZE - board_->square_size * var, board_->square_size * var * 2, board_->square_size * var * 2);
            path.arcTo(rect, -90, -270);
            board_->scene->addPath(path);
            board_->paths.push_back(path);
        }
        for (unsigned int y = 0; y < board_size_; y++) {
            for (unsigned int x = 0; x < board_size_; x++) {
                if (y < 2) {
                    (*board_)[x][y] = std::make_shared<SurakartaPiece>(x, y, p);
                    board_->scene->addItem((*board_)[x][y].get());
                } else if (y >= board_size_ - 2) {
                    (*board_)[x][y] = std::make_shared<SurakartaPiece>(x, y, ReverseColor(p));
                    board_->scene->addItem((*board_)[x][y].get());
                } else {
                    (*board_)[x][y] = std::make_shared<SurakartaPiece>(x, y, PieceColor::NONE);
                    board_->scene->addItem((*board_)[x][y].get());
                }
            }
        }
        game_info_->Reset();
    } else {
        std::ifstream fin(file_name);
        fin >> (*board_);
        fin >> (*game_info_);
        fin.close();
    }
    rule_manager_->OnUpdateBoard();
}

void SurakartaGame::SaveGame(std::string file_name) {
    std::ofstream fout(file_name);
    if(fout.is_open()){
        std::cout<<file_name<<std::endl;
        fout<<"tnnd"<<std::endl;
        fout << (*board_);
        fout << (*game_info_);
        fout.close();
    }
}

void SurakartaGame::UpdateGameInfo(SurakartaIllegalMoveReason move_reason, SurakartaEndReason end_reason, SurakartaPlayer winner) {
    if (move_reason == SurakartaIllegalMoveReason::LEGAL_CAPTURE_MOVE) {
        game_info_->last_captured_round_ = game_info_->num_round_;
    }
    if (!IsEndReason(end_reason)) {
        game_info_->current_player_ = ReverseColor(game_info_->current_player_);
        game_info_->num_round_++;
    } else {
        game_info_->end_reason_ = end_reason;
        game_info_->winner_ = winner;
    }
}

SurakartaMoveResponse SurakartaGame::Move(const SurakartaMove& move) {
    Recover_Color();
    (*board_)[move.from.x][move.from.y]->setSelect(false);
    (*board_)[move.to.x][move.to.y]->setSelect(false);
    SurakartaBoard::selected_num = 0;
    SurakartaIllegalMoveReason move_reason = rule_manager_->JudgeMove(move);
    auto [end_reason, winner] = rule_manager_->JudgeEnd(move_reason);

    UpdateGameInfo(move_reason, end_reason, winner);
    if (move_reason == SurakartaIllegalMoveReason::LEGAL_NON_CAPTURE_MOVE) {
        std::swap((*board_)[move.to.x][move.to.y], (*board_)[move.from.x][move.from.y]);
        (*board_)[move.to.x][move.to.y]->SetPosition(move.to);
        (*board_)[move.from.x][move.from.y]->SetPosition(move.from);
        rule_manager_->OnUpdateBoard();
    } else if (move_reason == SurakartaIllegalMoveReason::LEGAL_CAPTURE_MOVE) {
        /*board_->animation->setTargetObject((*board_)[move.from.x][move.from.y].get());
        board_->animation->setPropertyName("pos");
        QPointF from = (*board_)[move.from.x][move.from.y]->CoorDinate(),
            to = (*board_)[move.to.x][move.to.y]->CoorDinate();
        QPainterPath path = board_->paths[rule_manager_->circle];
        qreal percentage1 = findNearearPercentage(path, from),
            percentage2 = findNearearPercentage(path, to);
        board_->animation->setKeyValueAt(0, path.pointAtPercent(percentage1));
        board_->animation->setKeyValueAt(1, path.pointAtPercent(percentage2));
        board_->animation->start();*/
        (*board_)[move.to.x][move.to.y] = (*board_)[move.from.x][move.from.y];
        (*board_)[move.to.x][move.to.y]->SetPosition(move.to);
        (*board_)[move.from.x][move.from.y] = std::make_shared<SurakartaPiece>(move.from.x, move.from.y, PieceColor::NONE);
        rule_manager_->OnUpdateBoard();
    }

    SurakartaMoveResponse response(move_reason, end_reason, winner);
    return response;
}

void SurakartaGame::SetPlayer(QString play)
{
    if(play == "BLACK")
        p = SurakartaPlayer::BLACK;
    else if(play == "WHITE")
        p = SurakartaPlayer::WHITE;
    player = play;
}

void SurakartaGame::Recover_Color()
{
    for (unsigned int i = 0; i < board_size_; ++i) {
        for (unsigned int j = 0; j < board_size_; ++j) {
            (*board_)[i][j]->Recover_Color();
            board_->scene->update();
        }
    }
}

