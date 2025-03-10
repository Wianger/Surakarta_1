#ifndef SURAKARTAGAME_H
#define SURAKARTAGAME_H

#include <surakartarulemanager.h>
#include <QGraphicsItemAnimation>
#include <QTimeLine>

class SurakartaMoveResponse {
public:
    SurakartaMoveResponse(SurakartaIllegalMoveReason move_reason)
        : move_reason_(move_reason), end_reason_(SurakartaEndReason::NONE), winner_(SurakartaPlayer::NONE) {}

    SurakartaMoveResponse(SurakartaIllegalMoveReason move_reason, SurakartaEndReason end_reason, SurakartaPlayer winner)
        : move_reason_(move_reason), end_reason_(end_reason), winner_(winner) {}

    bool IsLegal() const {
        return IsLegalMoveReason(move_reason_);
    }

    bool IsEnd() const {
        return end_reason_ != SurakartaEndReason::NONE;
    }

    SurakartaIllegalMoveReason GetMoveReason() const {
        return move_reason_;
    }

    SurakartaEndReason GetEndReason() const {
        return end_reason_;
    }

    SurakartaPlayer GetWinner() const {
        return winner_;
    }

    //    private:
    SurakartaIllegalMoveReason move_reason_;
    SurakartaEndReason end_reason_;
    SurakartaPlayer winner_;
};

class SurakartaGame: public QObject{
    Q_OBJECT
public:
    SurakartaGame(QWidget *parent = nullptr)
        :board_(std::make_shared<SurakartaBoard>(parent)),
        game_info_(std::make_shared<SurakartaGameInfo>()),
        rule_manager_(std::make_shared<SurakartaRuleManager>(board_, game_info_)),
        is_captured(false) {
        animation = new QGraphicsItemAnimation;
        timeline = new QTimeLine(5000);
        timeline->setFrameRange(0, 10000);
        animation->setTimeLine(timeline);
    }


    /**
     * @brief Start the game. If file_name is empty, the board will be initialized to the initial state. Otherwise, the board will be initialized to the state in the file.
     */
    void StartGame(std::string file_name = "");

    /**
     * @brief Save the game to a file.
     * @param file_name The file name.
     */
    void SaveGame(std::string file_name);

    /**
     * @brief Update game info, this function should be called after each move.
     * @param reason SurakartaIllegalMoveReason.
     */
    void UpdateGameInfo(SurakartaIllegalMoveReason move_reason, SurakartaEndReason end_reason, SurakartaPlayer winner);

    /**
     * @brief Move a piece, and update game info.
     * @param move The move to be made.
     * @return See SurakartaMoveResponse.
     */
    SurakartaMoveResponse Move(const SurakartaMove& move, bool);

    std::shared_ptr<SurakartaBoard> GetBoard() const { return board_; }
    std::shared_ptr<SurakartaGameInfo> GetGameInfo() const { return game_info_; }
    bool IsEnd() const { return game_info_->IsEnd(); }

    void SetRuleManager(std::shared_ptr<SurakartaRuleManager> rule_manager) {  // For testing.
        rule_manager_ = rule_manager;
    }
    std::shared_ptr<SurakartaRuleManager> GetRuleManager() const { return rule_manager_; }  // For testing
    void Recover_Color();
    std::shared_ptr<SurakartaBoard> board_;
    std::shared_ptr<SurakartaGameInfo> game_info_;
    std::shared_ptr<SurakartaRuleManager> rule_manager_;
    QGraphicsItemAnimation *animation;
    QTimeLine *timeline;
    bool is_captured;
    QString player;
signals:
    void capture();  
};

#endif // SURAKARTAGAME_H
