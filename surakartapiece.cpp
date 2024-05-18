#include "surakartapiece.h"

SurakartaPiece::SurakartaPiece()
    : position_({0, 0}), color_(PieceColor::NONE), fixed_color_(PieceColor::NONE), is_selected(false) {}

SurakartaPiece::SurakartaPiece(unsigned int x, unsigned int y, PieceColor color)
    : position_({x, y}), color_(color), fixed_color_(color), is_selected(false) {}

SurakartaPiece::SurakartaPiece(SurakartaPosition position, PieceColor color)
    : position_(position), color_(color), fixed_color_(color), is_selected(false) {}

void SurakartaPiece::SetPosition(SurakartaPosition position)
{
    position_ = position;
    prepareGeometryChange();
}

void SurakartaPiece::SetColor(PieceColor color)
{
    color_ = color;
}

void SurakartaPiece::setSelect(bool t)
{
    is_selected = t;
}

void SurakartaPiece::setFixedColor(PieceColor color) { fixed_color_ = color; }

void SurakartaPiece::Set(SurakartaPosition position, PieceColor color) {
    position_ = position;
    color_ = color;
}

SurakartaPosition SurakartaPiece::GetPosition() const { return position_; }

PieceColor SurakartaPiece::GetColor() const { return color_; }

bool SurakartaPiece::GetSelect() const { return is_selected; }

std::ostream& operator<<(std::ostream& os, const SurakartaPiece& piece) {
    os << piece.color_;
    return os;
}

QRectF SurakartaPiece::boundingRect() const
{
    qreal x = GAP_SIZE + position_.x * SQUARE_SIZE - r;
    qreal y = GAP_SIZE + position_.y * SQUARE_SIZE - r;
    return QRectF(x, y, 2 * r, 2 * r);
}

void SurakartaPiece::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    if(color_ != PieceColor::NONE){
        if(color_ == PieceColor::WHITE)
            painter->setBrush(Qt::white);
        else if(color_ == PieceColor::BLACK)
            painter->setBrush(Qt::black);
        else if(color_ == PieceColor::YELLOW)
            painter->setBrush(Qt::yellow);
        painter->drawEllipse(boundingRect());
    }
}

void SurakartaPiece::Recover_Color() { color_ = fixed_color_; }

QPointF SurakartaPiece::CoorDinate()
{
    qreal x = GAP_SIZE + position_.x * SQUARE_SIZE;
    qreal y = GAP_SIZE + position_.y * SQUARE_SIZE;
    return QPointF(x, y);
}
