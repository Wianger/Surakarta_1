#ifndef SURAKARTAPIECE_H
#define SURAKARTAPIECE_H

#include <QGraphicsEllipseItem>
#include <QPainter>
#include <iostream>
#include <QGraphicsSceneMouseEvent>
#include <surakartacommon.h>

class SurakartaPiece : public QGraphicsObject
{
    Q_OBJECT
public:
    SurakartaPiece();
    SurakartaPiece(unsigned int x, unsigned int y, PieceColor color);
    SurakartaPiece(SurakartaPosition position, PieceColor color);
    void SetPosition(SurakartaPosition position);
    void SetColor(PieceColor color);
    void setSelect(bool);
    void setFixedColor(PieceColor color);
    void Set(SurakartaPosition position, PieceColor color);
    SurakartaPosition GetPosition() const;
    PieceColor GetColor() const;
    PieceColor GetFixColor() {return fixed_color_;}
    bool GetSelect() const;
    friend std::ostream& operator<<(std::ostream& os, const SurakartaPiece& piece);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
    void Recover_Color();
    QPointF CoorDinate();

private:
    SurakartaPosition position_;
    PieceColor color_;
    PieceColor fixed_color_;
    unsigned int r = 180 / BOARDSIZE;
    bool is_selected;
};

#endif // SURAKARTAPIECE_H
