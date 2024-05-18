#ifndef SURAKARTABOARD_H
#define SURAKARTABOARD_H

#include <QGraphicsView>
#include <memory>
#include <vector>
#include "surakartapiece.h"
#include <QMouseEvent>
#include <QWidget>
#include <QGraphicsItemAnimation>
#include <QTimeLine>

class SurakartRow : public std::vector<std::shared_ptr<SurakartaPiece>> {
public:
    SurakartRow(unsigned int board_size)
        : std::vector<std::shared_ptr<SurakartaPiece>>(board_size) {}
};


class SurakartaBoard : public QGraphicsView, public std::vector<SurakartRow>
{
    Q_OBJECT
public:
    unsigned int board_size_;
    std::vector<QPainterPath> paths;
    QGraphicsScene *scene;
    static unsigned int selected_num;
    //QGraphicsItemAnimation *animation;
    //QTimeLine *timeline;
    static SurakartaPosition from, to;

    explicit SurakartaBoard(QWidget *parent = nullptr);
    bool IsInside(const SurakartaPosition& position) const;
    friend inline std::ostream& operator<<(std::ostream& os, const SurakartaBoard& board) {
        for (unsigned int y = 0; y < board.board_size_; y++) {
            for (unsigned int x = 0; x < board.board_size_; x++) {
                os << (*board[x][y]) << " ";
            }
            os << std::endl;
        }
        return os;
    }

    friend inline std::istream& operator>>(std::istream& is, SurakartaBoard& board) {
        for (unsigned int y = 0; y < board.board_size_; y++) {
            for (unsigned int x = 0; x < board.board_size_; x++) {
                char ch;
                is >> ch;
                PieceColor color;
                switch (ch) {
                case 'B':
                    color = PieceColor::BLACK;
                    break;
                case 'W':
                    color = PieceColor::WHITE;
                    break;
                default:
                    color = PieceColor::NONE;
                    break;
                }
                board[x][y] = std::make_shared<SurakartaPiece>(x, y, color);
            }
        }
        return is;
    }
protected:
    void mousePressEvent(QMouseEvent *event);
};

#endif // SURAKARTABOARD_H
