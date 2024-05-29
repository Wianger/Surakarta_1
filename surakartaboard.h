#ifndef SURAKARTABOARD_H
#define SURAKARTABOARD_H

#include <QGraphicsView>
#include <memory>
#include <vector>
#include "surakartapiece.h"
#include <QMouseEvent>
#include <QWidget>
#include <QPropertyAnimation>

class SurakartRow : public std::vector<std::shared_ptr<SurakartaPiece>> {
public:
    SurakartRow()
        : std::vector<std::shared_ptr<SurakartaPiece>>(BOARDSIZE) {}
};


class SurakartaBoard : public QGraphicsView, public std::vector<SurakartRow>
{
    Q_OBJECT
public:
    std::vector<QPainterPath> paths;
    QGraphicsScene *scene;
    static unsigned int selected_num;
    static SurakartaPosition from, to;

    explicit SurakartaBoard(QWidget *parent = nullptr);
    bool IsInside(const SurakartaPosition& position) const;
    friend inline std::ostream& operator<<(std::ostream& os, const SurakartaBoard& board) {
        for (unsigned int y = 0; y < BOARDSIZE; y++) {
            for (unsigned int x = 0; x < BOARDSIZE; x++) {
                os << (*board[x][y]) << " ";
            }
            os << std::endl;
        }
        return os;
    }

    friend inline std::istream& operator>>(std::istream& is, SurakartaBoard& board) {
        for (unsigned int y = 0; y < BOARDSIZE; y++) {
            for (unsigned int x = 0; x < BOARDSIZE; x++) {
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
signals:
    void promt();
    void cancle();
    void move();
};

#endif // SURAKARTABOARD_H
