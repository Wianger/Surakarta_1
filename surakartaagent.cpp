#include "surakartaagent.h"
#include <algorithm>
#include <vector>
#include "GlobalRandomGenerator.h"
#include "surakartacommon.h"

SurakartaMove SurakartaAgent::CalculateMove() {
    std::vector<SurakartaPosition> from;
    std::vector<SurakartaPosition> to;
    for (unsigned int i = 0; i < BOARDSIZE; i++) {
        for (unsigned int j = 0; j < BOARDSIZE; j++) {
            SurakartaPosition position = {i, j};
            if ((*board_)[i][j]->GetColor() == game_info_->current_player_) {
                from.push_back(position);
            } else {
                to.push_back(position);
            }
        }
    }
    std::shuffle(from.begin(), from.end(), GlobalRandomGenerator::getInstance());
    std::shuffle(to.begin(), to.end(), GlobalRandomGenerator::getInstance());
    SurakartaMove rd_move({0, 0}, {0, 0}, game_info_->current_player_);
    for (auto& p1 : from) {
        for (auto& p2 : to) {
            SurakartaMove move = {p1, p2, game_info_->current_player_};
            SurakartaIllegalMoveReason reason = rule_manager_->JudgeMove(move);
            if (reason == SurakartaIllegalMoveReason::LEGAL_CAPTURE_MOVE) {
                return move;
            } else if (reason == SurakartaIllegalMoveReason::LEGAL_NON_CAPTURE_MOVE) {
                rd_move = move;
            }
        }
    }
    return rd_move;
}
