#include "surakartagame.h"
#include <fstream>
#include <iostream>
#include <cmath>

double calculateProportionOnPath(const QPainterPath &path, const QPointF &point) {
    qreal minDistance = std::numeric_limits<qreal>::max();
    qreal proportion = 0.0;
    const int segments = 1000;
    for (int i = 0; i <= segments; ++i) {
        qreal percent = static_cast<qreal>(i) / segments;
        QPointF currentPoint = path.pointAtPercent(percent);
        qreal distance = QLineF(point, currentPoint).length();
        if (distance < minDistance) {
            minDistance = distance;
            proportion = percent;
        }
    }

    return proportion;
}

QPainterPath DrawLine(unsigned int SQUARE_SIZE, unsigned int var)
{
    QRectF rect(GAP_SIZE, GAP_SIZE, SIZE - GAP_SIZE * 2, SIZE - GAP_SIZE * 2);
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
    return path;
}

void SurakartaGame::StartGame(std::string file_name) {
    if (file_name.empty()) {
        QPen pen;
        pen.setWidth(2);
        QBrush brush;
        brush.setColor(Qt::white);
        QRectF rect(GAP_SIZE, GAP_SIZE, SIZE - GAP_SIZE * 2, SIZE - GAP_SIZE * 2);
        board_->scene->addRect(rect, pen, brush);
        unsigned int SQUARE_SIZE = (SIZE - 2 * GAP_SIZE) / (BOARDSIZE - 1);
        for (unsigned int var = 1; var < BOARDSIZE / 2; ++var) {
            QPainterPath path = DrawLine(SQUARE_SIZE, var);
            board_->scene->addPath(path);
            board_->paths.push_back(path);
        }
        for (unsigned int y = 0; y < BOARDSIZE; y++) {
            for (unsigned int x = 0; x < BOARDSIZE; x++) {
                if (y < 2) {
                    (*board_)[x][y] = std::make_shared<SurakartaPiece>(x, y, PieceColor::BLACK);
                    board_->scene->addItem((*board_)[x][y].get());
                } else if (y >= BOARDSIZE - 2) {
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

void SurakartaGame::SaveGame(std::string file_name) {
    std::ofstream fout(file_name);
    if(fout.is_open()){
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

SurakartaMoveResponse SurakartaGame::Move(const SurakartaMove& move, bool is_ai) {
    Recover_Color();
    (*board_)[move.from.x][move.from.y]->setSelect(false);
    (*board_)[move.to.x][move.to.y]->setSelect(false);
    SurakartaBoard::selected_num = 0;
    SurakartaIllegalMoveReason move_reason = rule_manager_->JudgeMove(move);
    std::cout<<move_reason<<std::endl;
    auto [end_reason, winner] = rule_manager_->JudgeEnd(move_reason);
    UpdateGameInfo(move_reason, end_reason, winner);
    if (move_reason == SurakartaIllegalMoveReason::LEGAL_NON_CAPTURE_MOVE) {
        std::swap((*board_)[move.to.x][move.to.y], (*board_)[move.from.x][move.from.y]);
        (*board_)[move.to.x][move.to.y]->SetPosition(move.to);
        (*board_)[move.from.x][move.from.y]->SetPosition(move.from);
        rule_manager_->OnUpdateBoard();
    } else if (move_reason == SurakartaIllegalMoveReason::LEGAL_CAPTURE_MOVE) {
        if(!is_ai){
            animation->setItem((*board_)[move.from.x][move.from.y].get());
            QPainterPath path = board_->paths[rule_manager_->circle];
            QPointF from = (*board_)[move.from.x][move.from.y]->CoorDinate();
            QPointF to = (*board_)[move.to.x][move.to.y]->CoorDinate();
            double p1 = calculateProportionOnPath(path, from);
            double p2 = calculateProportionOnPath(path, to);
            timeline->start();
            if(rule_manager_->clock == 1){
                //std::cout<<p1<<" "<<p2<<std::endl;
                is_captured = true;
                int k = 0;
                while(std::fabs(p1 - p2) >= 0.0001)
                {
                    QPointF pos = path.pointAtPercent(p1) - from;
                    animation->setPosAt(k / 10000.0, pos);
                    //std::cout<<"ENTER1"<<std::endl;
                    //std::cout<<p1<<std::endl;
                    p1 += 0.0001, k++;
                    if(p1 >= 1)
                        p1 = 0;
                    if(k >= 10000)
                        k = 0;
                }
            }
            else if(rule_manager_->clock == -1){
                is_captured = true;
                int k = 0;
                //std::cout<<p1<<" "<<p2<<std::endl;
                while(std::fabs(p1 - p2) >= 0.0001)
                {
                    QPointF pos = path.pointAtPercent(p1) - from;
                    animation->setPosAt(k / 10000.0, pos);
                    //std::cout<<"ENTER2"<<std::endl;
                    //std::cout<<p1<<std::endl;
                    p1 -= 0.0001, k++;
                    if(p1 <= 0)
                        p1 = 1;
                    if(k >= 10000)
                        k = 0;
                }
            }
        }
        if(is_ai){
            std::cout<<"AI"<<std::endl;
            (*board_)[move.to.x][move.to.y] = (*board_)[move.from.x][move.from.y];
            (*board_)[move.to.x][move.to.y]->SetPosition(move.to);
            (*board_)[move.from.x][move.from.y] = std::make_shared<SurakartaPiece>(move.from.x, move.from.y, PieceColor::NONE);
            board_->scene->addItem((*board_)[move.from.x][move.from.y].get());
            rule_manager_->OnUpdateBoard();
        }
    }

    SurakartaMoveResponse response(move_reason, end_reason, winner);
    return response;
}

void SurakartaGame::Recover_Color()
{
    for (unsigned int i = 0; i < BOARDSIZE; ++i) {
        for (unsigned int j = 0; j < BOARDSIZE; ++j) {
            (*board_)[i][j]->Recover_Color();
            board_->scene->update();
        }
    }
}

