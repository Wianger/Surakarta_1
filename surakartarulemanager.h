#ifndef SURAKARTARULEMANAGER_H
#define SURAKARTARULEMANAGER_H

#include <surakartaboard.h>
#include <surakartacommon.h>

class SurakartaRuleManager {
public:
    SurakartaRuleManager() = default;

    SurakartaRuleManager(std::shared_ptr<SurakartaBoard> board,
                         std::shared_ptr<SurakartaGameInfo> game_info)
        : board_size_(board->board_size_),
        board_(std::const_pointer_cast<const SurakartaBoard>(board)),
        game_info_(std::const_pointer_cast<const SurakartaGameInfo>(game_info)),
        circle(0) {}

    unsigned int GetBoardSize() {
        return board_size_;
    }

    virtual void OnUpdateBoard();

    /**
     * @brief Judge whether a move is legal.
     * @param move The move to be judged.
     */
    virtual SurakartaIllegalMoveReason JudgeMove(const SurakartaMove& move);

    /**
     * @brief Judge whether the game is end.
     * @param reason SurakartaIllegalMoveReason of the last move.
     */
    virtual std::pair<SurakartaEndReason, SurakartaPlayer> JudgeEnd(const SurakartaIllegalMoveReason reason);

    /**
     * @brief Get all legal target positions of a piece.
     * @param postion The position of the piece.
     */
    virtual std::unique_ptr<std::vector<SurakartaPosition>> GetAllLegalTarget(const SurakartaPosition postion);
    SurakartaPosition Row_Line(SurakartaPosition, unsigned int);

    //    protected:
    unsigned int board_size_;
    std::shared_ptr<const SurakartaBoard> board_;
    std::shared_ptr<const SurakartaGameInfo> game_info_;
    unsigned int circle;
};

#endif // SURAKARTARULEMANAGER_H
