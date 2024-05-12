#include "surakartaboard.h"

SurakartaBoard::SurakartaBoard(QWidget *parent)
    : QGraphicsView{parent}{
    board_size_ = BOARD_SIZE;
    for (unsigned int i = 0; i < board_size_; i++) {
        this->push_back(SurakartRow(board_size_));
    }
    scene = new QGraphicsScene(0, 0, SIZE, SIZE);
    setScene(scene);
    animation = new QGraphicsItemAnimation;
    timeline = new QTimeLine(10000);
    timeline->setFrameRange(1, 1000);
    timeline->setLoopCount(1);
    animation->setTimeLine(timeline);
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
    /*if(piece)
        std::cout<<typeid(*piece).name()<<std::endl;
    else
        std::cout<<"PressEorror"<<std::endl;*/
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
