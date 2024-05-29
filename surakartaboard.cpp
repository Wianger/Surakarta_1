#include "surakartaboard.h"

SurakartaBoard::SurakartaBoard(QWidget *parent)
    : QGraphicsView{parent}{
    for (unsigned int i = 0; i < BOARDSIZE; i++) {
        this->push_back(SurakartRow());
    }
    scene = new QGraphicsScene(0, 0, SIZE, SIZE);
    setScene(scene);
}

unsigned int SurakartaBoard::selected_num = 0;
SurakartaPosition SurakartaBoard::from = SurakartaPosition(-1, -1);
SurakartaPosition SurakartaBoard::to = SurakartaPosition(-1, -1);

bool SurakartaBoard::IsInside(const SurakartaPosition& position) const {
    return position.x < BOARDSIZE && position.y < BOARDSIZE;
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
            emit promt();
            scene->update();
        }
        else if(selected_num == 1){
            if(surakartaPiece->GetSelect()) {
                surakartaPiece->Recover_Color();
                surakartaPiece->setSelect(false);
                from = SurakartaPosition(-1, -1);
                selected_num --;
                emit cancle();
                scene->update();
            }
            else{
                surakartaPiece->setSelect(true);
                to = surakartaPiece->GetPosition();
                selected_num ++;
                emit move();
                scene->update();
            }
        }
    }
    QGraphicsView::mousePressEvent(event);
}
