#include "surakartagame.h"
#include <fstream>

void SurakartaGame::StartGame(std::string file_name) {
    if (file_name.empty()) {
        QPen pen;
        pen.setWidth(2);
        QBrush brush;
        brush.setColor(Qt::white);
        QRectF rect(GAP_SIZE, GAP_SIZE, SIZE - GAP_SIZE * 2, SIZE - GAP_SIZE * 2);
        board_->scene->addRect(rect, pen, brush);
        for (int var = 1; var < BOARD_SIZE / 2; ++var) {
            QPainterPath path;
            path.moveTo(QPointF(GAP_SIZE + SQUARE_SIZE * var, GAP_SIZE));
            path.lineTo(QPointF(GAP_SIZE + SQUARE_SIZE * var, SIZE - GAP_SIZE));
            rect.setRect(GAP_SIZE - SQUARE_SIZE * var, SIZE - GAP_SIZE - SQUARE_SIZE * var, SQUARE_SIZE * var * 2, SQUARE_SIZE * var * 2);
            path.arcTo(rect, 0, -270);
            path.arcMoveTo(rect, 90);
            path.lineTo(SIZE - GAP_SIZE, SIZE - GAP_SIZE - SQUARE_SIZE * var);
            rect.setRect(SIZE - GAP_SIZE - SQUARE_SIZE * var, SIZE - GAP_SIZE - SQUARE_SIZE * var, SQUARE_SIZE * var * 2, SQUARE_SIZE * var * 2);
            path.arcTo(rect, 90, -270);
            path.arcMoveTo(rect, 180);
            path.lineTo(SIZE - GAP_SIZE - SQUARE_SIZE * var, GAP_SIZE);
            rect.setRect(SIZE - GAP_SIZE - SQUARE_SIZE * var, GAP_SIZE - SQUARE_SIZE * var, SQUARE_SIZE * var * 2, SQUARE_SIZE * var * 2);
            path.arcTo(rect, 180, -270);
            path.arcMoveTo(rect, 270);
            path.lineTo(GAP_SIZE, GAP_SIZE + SQUARE_SIZE * var);
            rect.setRect(GAP_SIZE - SQUARE_SIZE * var, GAP_SIZE - SQUARE_SIZE * var, SQUARE_SIZE * var * 2, SQUARE_SIZE * var * 2);
            path.arcTo(rect, -90, -270);
            board_->scene->addPath(path);
            board_->paths.push_back(path);
        }
        for (unsigned int y = 0; y < board_size_; y++) {
            for (unsigned int x = 0; x < board_size_; x++) {
                if (y < 2) {
                    (*board_)[x][y] = std::make_shared<SurakartaPiece>(x, y, PieceColor::BLACK);
                    board_->scene->addItem((*board_)[x][y].get());
                } else if (y >= board_size_ - 2) {
                    (*board_)[x][y] = std::make_shared<SurakartaPiece>(x, y, PieceColor::WHITE);
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

void SurakartaGame::SaveGame(std::string file_name) const {
    std::ofstream fout(file_name);
    fout << (*board_);
    fout << (*game_info_);
    fout.close();
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
    (*board_)[move.from.x][move.from.y]->Recover_Color();
    (*board_)[move.from.x][move.from.y]->setSelect(false);
    (*board_)[move.to.x][move.to.y]->Recover_Color();
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
        (*board_)[move.to.x][move.to.y] = (*board_)[move.from.x][move.from.y];
        (*board_)[move.to.x][move.to.y]->SetPosition(move.to);
        (*board_)[move.from.x][move.from.y] = std::make_shared<SurakartaPiece>(move.from.x, move.from.y, PieceColor::NONE);
        rule_manager_->OnUpdateBoard();
    }

    SurakartaMoveResponse response(move_reason, end_reason, winner);
    return response;
}

/*void SurakartaGame::Animation(const SurakartaMove& move)
{
    board_->animation->setItem((*board_)[move.from.x][move.from.y].get());
    QPointF pos;
    QPointF from = (*board_)[move.from.x][move.from.y]->CoorDinate();
    QPointF to = (*board_)[move.to.x][move.to.y]->CoorDinate();
    to.setX(to.x() - from.x()), to.setY(to.y() - from.y());
    board_->timeline->start();
    std::cout<<rule_manager_->clock<<std::endl;
    if(rule_manager_->clock == 1){
        for (int var = 0; var <= 1000 && (abs(pos.x() - to.x()) > EPS || abs(pos.y() - to.y()) > EPS); ++var) {
            //std::cout<<pos.x()<<" "<<pos.y()<<std::endl;
            //std::cout<<to.x()<<" "<<to.y()<<std::endl;
            pos.setX(board_->paths[rule_manager_->circle].pointAtPercent(var / 1000.0).x() - from.x()), pos.setY(board_->paths[rule_manager_->circle].pointAtPercent(var / 1000.0).y() - from.y());
            board_->animation->setPosAt(var / 1000.0, pos);
        }
    }
    else if(rule_manager_->clock == -1){
        for (int var = 1000, i = 0; var >= 0 && (abs(pos.x() - to.x()) > EPS || abs(pos.y() - to.y()) > EPS); --var, ++i) {
            //std::cout<<pos.x()<<" "<<pos.y()<<std::endl;
            //std::cout<<to.x()<<" "<<to.y()<<std::endl;
            pos.setX(board_->paths[rule_manager_->circle].pointAtPercent(var / 1000.0).x() - from.x()), pos.setY(board_->paths[rule_manager_->circle].pointAtPercent(var / 1000.0).y() - from.y());
            board_->animation->setPosAt(i / 1000.0, pos);
        }
    }
    board_->scene->update();
}*/
