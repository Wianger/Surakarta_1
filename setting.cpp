#include "setting.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QMessageBox>

Setting::Setting(QWidget *parent) : QDialog(parent)
{
    QLabel *boardsizeLabel = new QLabel("BoardSize:", this);
    boardsizeEdit = new QLineEdit(this);
    boardsizeEdit->setText("6");
    QLabel *countdownLabel = new QLabel("CountDown:", this);
    countdownEdit = new QLineEdit(this);
    countdownEdit->setText("3");
    QLabel *roundLabel = new QLabel("CountDown:", this);
    roundEdit = new QLineEdit(this);
    roundEdit->setText("40");
    confirmButton = new QPushButton("Confirm", this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(boardsizeLabel);
    layout->addWidget(boardsizeEdit);
    layout->addWidget(countdownLabel);
    layout->addWidget(countdownEdit);
    layout->addWidget(roundLabel);
    layout->addWidget(roundEdit);
    layout->addWidget(confirmButton);
    setLayout(layout);
    connect(confirmButton, &QPushButton::clicked, this, &Setting::onconfirmClicked);
}

void Setting::setInfo(QString u, QString p, QString r, QString po)
{
    username = u;
    player = p;
    room = r;
    port = po;
}

void Setting::onconfirmClicked()
{
    if(!boardsizeEdit->text().isEmpty() && !countdownEdit->text().isEmpty())
    {
        unsigned int boardsize = boardsizeEdit->text().toInt(),
            countdown = countdownEdit->text().toInt(),
            round = roundEdit->text().toInt();
        if(boardsize % 2 != 0)
            QMessageBox::information(this, "Fail", "请检查棋盘大小");
        else{
            accept();
            w = new MainWindow(boardsize, countdown, player, username, room, port.toInt(), round);
            w->connectToServer();
            w->show();
        }
    }
}
