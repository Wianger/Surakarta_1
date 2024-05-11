#include "surakartagame.h"
#include <fstream>
#include <iostream>

void SurakartaGame::StartGame(std::string file_name) {
    if (file_name.empty()) {
        QPen pen;
        pen.setWidth(2);
        QBrush brush;
        brush.setColor(Qt::white);
        QRectF rect(GAP_SIZE, GAP_SIZE, SIZE - GAP_SIZE * 2, SIZE - GAP_SIZE * 2);
        board_->scene->addRect(rect, pen, brush);
        for (int var = 1; var < BOARD_SIZE / 2; ++var) {
            QPainterPath line;
            line.moveTo(QPointF(GAP_SIZE + SQUARE_SIZE * var, GAP_SIZE));
            line.lineTo(QPointF(GAP_SIZE + SQUARE_SIZE * var, SIZE - GAP_SIZE));
            rect.setRect(GAP_SIZE - SQUARE_SIZE * var, SIZE - GAP_SIZE - SQUARE_SIZE * var, SQUARE_SIZE * var * 2, SQUARE_SIZE * var * 2);
            line.arcMoveTo(rect, 90);
            line.lineTo(SIZE - GAP_SIZE, SIZE - GAP_SIZE - SQUARE_SIZE * var);
            line.arcTo(rect, 90, 270);
            rect.setRect(SIZE - GAP_SIZE - SQUARE_SIZE * var, SIZE - GAP_SIZE - SQUARE_SIZE * var, SQUARE_SIZE * var * 2, SQUARE_SIZE * var * 2);
            line.arcMoveTo(rect, 180);
            line.lineTo(SIZE - GAP_SIZE - SQUARE_SIZE * var, GAP_SIZE);
            line.arcTo(rect, -180, 270);
            rect.setRect(SIZE - GAP_SIZE - SQUARE_SIZE * var, GAP_SIZE - SQUARE_SIZE * var, SQUARE_SIZE * var * 2, SQUARE_SIZE * var * 2);
            line.arcMoveTo(rect, 270);
            line.lineTo(GAP_SIZE, GAP_SIZE + SQUARE_SIZE * var);
            line.arcTo(rect, -90, 270);
            rect.setRect(GAP_SIZE - SQUARE_SIZE * var, GAP_SIZE - SQUARE_SIZE * var, SQUARE_SIZE * var * 2, SQUARE_SIZE * var * 2);
            line.arcTo(rect, 0, 270);
            board_->scene->addPath(line);
            board_->lines.push_back(line);
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

    std::cout<<"move"<<std::endl;
    std::cout<<move_reason<<std::endl;
    if (move_reason == SurakartaIllegalMoveReason::LEGAL_NON_CAPTURE_MOVE) {
        std::cout<<"enter"<<std::endl;
        std::swap((*board_)[move.to.x][move.to.y], (*board_)[move.from.x][move.from.y]);
        (*board_)[move.from.x][move.from.y]->SetPosition(move.from);
        (*board_)[move.to.x][move.to.y]->SetPosition(move.to);
        board_->scene->update();
        rule_manager_->OnUpdateBoard();
    } else if (move_reason == SurakartaIllegalMoveReason::LEGAL_CAPTURE_MOVE) {

        rule_manager_->OnUpdateBoard();
    }
    else{
        board_->scene->update();
    }

    SurakartaMoveResponse response(move_reason, end_reason, winner);
    return response;
}



