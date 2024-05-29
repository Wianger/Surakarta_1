#ifndef SURAKARTACOMMON_H
#define SURAKARTACOMMON_H

#include <iostream>
#include <QString>

#define SIZE 800
#define GAP_SIZE 200
extern unsigned int BOARDSIZE;
extern unsigned int MAX_NO_CAPTURE_ROUND;
extern unsigned int CountDown;

using PieceColorMemoryType = int;
enum class PieceColor : PieceColorMemoryType { BLACK,
                                               WHITE,
                                               YELLOW,
                                               RED,
                                               NONE,
                                               UNKNOWN
};

using SurakartaPlayer = PieceColor;

inline PieceColor ReverseColor(PieceColor color) {
    switch (color) {
    case PieceColor::BLACK:
        return PieceColor::WHITE;
    case PieceColor::WHITE:
        return PieceColor::BLACK;
    default:
        return color;
    }
}

inline std::ostream& operator<<(std::ostream& os, const PieceColor& color) {
    switch (color) {
    case PieceColor::NONE:
        os << ".";
        break;
    case PieceColor::BLACK:
        os << "B";
        break;
    case PieceColor::WHITE:
        os << "W";
        break;
    default:
        os << "?";
        break;
    }
    return os;
}

inline std::istream& operator>>(std::istream& is, PieceColor& color) {
    char ch;
    is >> ch;
    switch (ch) {
    case '.':
        color = PieceColor::NONE;
        break;
    case 'B':
        color = PieceColor::BLACK;
        break;
    case 'W':
        color = PieceColor::WHITE;
        break;
    default:
        color = PieceColor::UNKNOWN;
        break;
    }
    return is;
}

enum class SurakartaIllegalMoveReason {
    LEGAL,                     // unused
    LEGAL_CAPTURE_MOVE,        // capture a opponent's piece, and the move consists at least one corner loop
    LEGAL_NON_CAPTURE_MOVE,    // just as the name
    ILLIGAL,                   // unused
    NOT_PLAYER_TURN,           // move when it's not the player's turn.
    OUT_OF_BOARD,              // from or to position is out of board
    NOT_PIECE,                 // move a position that is not a piece
    NOT_PLAYER_PIECE,          // move a piece that is not the player's
    ILLIGAL_CAPTURE_MOVE,      // try to capture a opponent's piece, but the move can't consist any corner loop
    ILLIGAL_NON_CAPTURE_MOVE,  // otherwise
    GAME_ALREADY_END,          // unused
    GAME_NOT_START,       // unused
    TIMEOUT
};

enum class SurakartaEndReason {
    NONE,          // not end
    STALEMATE,     // both players can't make more move
    CHECKMATE,     // one player's all pieces are captured
    TRAPPED,       // unused, one player's pieces are all trapped, no legal move can be made.
    RESIGN,        // one player resigns.
    TIMEOUT,       // one player's time is out.
    ILLIGAL_MOVE,    // one player makes an illegal move
};

bool IsLegalMoveReason(SurakartaIllegalMoveReason reason);
bool IsEndReason(SurakartaEndReason reason);
std::ostream& operator<<(std::ostream& os, const SurakartaIllegalMoveReason& reason);
std::istream& operator>>(std::istream& is, SurakartaIllegalMoveReason& reason);

std::ostream& operator<<(std::ostream& os, const SurakartaEndReason& reason);
std::istream& operator>>(std::istream& is, SurakartaEndReason& reason);

struct SurakartaPosition {
    unsigned int x;
    unsigned int y;
    SurakartaPosition(unsigned int x = 0, unsigned int y = 0)
        : x(x), y(y) {}
    bool operator==(const SurakartaPosition& rhs) const {
        return x == rhs.x && y == rhs.y;
    }
    bool operator!=(const SurakartaPosition& rhs) const {
        return !(*this == rhs);
    }
};

struct SurakartaMove {
    SurakartaPosition from;
    SurakartaPosition to;
    SurakartaPlayer player;
    SurakartaMove()
        : from(-1, -1), to(-1, -1), player(SurakartaPlayer::NONE) {}
    SurakartaMove(unsigned int from_x, unsigned int from_y, unsigned int to_x, unsigned int to_y, SurakartaPlayer player)
        : from(from_x, from_y), to(to_x, to_y), player(player) {}
    SurakartaMove(SurakartaPosition from, SurakartaPosition to, SurakartaPlayer player)
        : from(from), to(to), player(player) {}
};

struct SurakartaGameInfo {
    SurakartaPlayer current_player_;
    unsigned int num_round_;
    unsigned int last_captured_round_;
    SurakartaEndReason end_reason_;
    SurakartaPlayer winner_;

    SurakartaGameInfo()
        : current_player_(SurakartaPlayer::BLACK),
        num_round_(1),
        last_captured_round_(0),
        end_reason_(SurakartaEndReason::NONE),
        winner_(SurakartaPlayer::NONE) {}

    void Reset() {
        current_player_ = SurakartaPlayer::BLACK;
        num_round_ = 1;
        last_captured_round_ = 0;
        end_reason_ = SurakartaEndReason::NONE;
        winner_ = SurakartaPlayer::NONE;
    }

    bool IsEnd() const { return IsEndReason(end_reason_); }
    SurakartaPlayer Winner() const { return winner_; }

    friend std::ostream& operator<<(std::ostream& os, const SurakartaGameInfo& game_info) {
        os << "current_player: " << game_info.current_player_ << std::endl;
        os << "num_round: " << game_info.num_round_ << std::endl;
        os << "last_captured_round: " << game_info.last_captured_round_ << std::endl;
        os << "end_reason: " << game_info.end_reason_ << std::endl;
        os << "winner: " << game_info.winner_ << std::endl;
        return os;
    }

    friend std::istream& operator>>(std::istream& is, SurakartaGameInfo& game_info) {
        std::string str;
        is >> str >> game_info.current_player_;
        is >> str >> game_info.num_round_;
        is >> str >> game_info.last_captured_round_;
        is >> str >> game_info.end_reason_;
        is >> str >> game_info.winner_;
        return is;
    }
};

class SurakartaException : public std::exception {
public:
    SurakartaException(const std::string& message)
        : message_(message) {}
    virtual const char* what() const throw() { return message_.c_str(); }

private:
    std::string message_;
};

inline QString player2str(SurakartaPlayer pl)
{
    QString player;
    if(pl == PieceColor::BLACK)
        player = "BLACK";
    else
        player = "WHITE";
    return player;
}

inline QString endReason2String(SurakartaEndReason endReason) {
    switch (endReason) {
    case SurakartaEndReason::NONE:
        return "NONE";
    case SurakartaEndReason::STALEMATE:
        return "STALEMATE";
    case SurakartaEndReason::CHECKMATE:
        return "CHECKMATE";
    case SurakartaEndReason::TRAPPED:
        return "TRAPPED";
    case SurakartaEndReason::RESIGN:
        return "RESIGN";
    case SurakartaEndReason::TIMEOUT:
        return "TIMEOUT";
    case SurakartaEndReason:: ILLIGAL_MOVE:
        return "ILLEGAL_MOVE";
    default:
        return "UNKNOWN";
    }
}
#endif // SURAKARTACOMMON_H
