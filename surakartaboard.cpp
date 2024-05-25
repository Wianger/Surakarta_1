#include "surakartaboard.h"

SurakartaBoard::SurakartaBoard(QWidget *parent, unsigned int boardsize)
    : QGraphicsView{parent}{
    board_size_ = boardsize;
    square_size = (SIZE - 2 * GAP_SIZE) / (board_size_ - 1);
    SurakartaPiece::BOARD_SIZE = boardsize;
    SurakartaPiece::SQUARE_SIZE = (SIZE - 2 * GAP_SIZE) / (boardsize - 1);
    for (unsigned int i = 0; i < board_size_; i++) {
        this->push_back(SurakartRow(board_size_));
    }
    scene = new QGraphicsScene(0, 0, SIZE, SIZE);
    setScene(scene);
    animation = new QPropertyAnimation;
    animation->setDuration(5000);
}

unsigned int SurakartaBoard::selected_num = 0;
SurakartaPosition SurakartaBoard::from = SurakartaPosition(-1, -1);
SurakartaPosition SurakartaBoard::to = SurakartaPosition(-1, -1);

bool SurakartaBoard::IsInside(const SurakartaPosition& position) const {
    return position.x < board_size_ && position.y < board_size_;
}

void SurakartaBoard::mousePressEvent(QMouseEvent *event)
{
    QPointF pos = mapToScene(event->pos());
    QGraphicsItem* piece = itemAt(pos.x(), pos.y());
    SurakartaPiece *surakartaPiece = dynamic_cast<SurakartaPiece*>(piece);
    if(surakartaPiece){
        if(!selected_num) {
            surakartaPiece->SetColor(PieceColor::YELLOW);
            surakartaPiece->setSelect(true);
            from = surakartaPiece->GetPosition();
            selected_num ++;
            scene->update();
        }
        else if(selected_num == 1){
            if(surakartaPiece->GetSelect()) {
                surakartaPiece->Recover_Color();
                surakartaPiece->setSelect(false);
                from = SurakartaPosition(-1, -1);
                selected_num --;
                scene->update();
            }
            else{
                surakartaPiece->SetColor(PieceColor::YELLOW);
                surakartaPiece->setSelect(true);
                to = surakartaPiece->GetPosition();
                selected_num ++;
                scene->update();
            }
        }
        else if(selected_num == 2)
        {
            if(surakartaPiece->GetPosition() == to){
                surakartaPiece->Recover_Color();
                surakartaPiece->setSelect(false);
                to = SurakartaPosition(-1, -1);
                selected_num --;
                scene->update();
            }
        }
    }
    QGraphicsView::mousePressEvent(event);
}
