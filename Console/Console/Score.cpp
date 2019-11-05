#include "Score.h"
#include <QFont>

Score::Score(QGraphicsItem * parent) :
    QGraphicsTextItem(parent),
    playerName_(""),
    score_(0)
{
    setPlainText(playerName_ + QString(" Score: ") +
                 QString::number(score_));
    setDefaultTextColor(Qt::blue);
    setFont(QFont("times",16));
}

void Score::increase()
{
    score_++;
    setPlainText(playerName_ + QString(" Score: ") +
                 QString::number(score_));
}

void Score::decrease(){
    score_--;
    setPlainText(playerName_ + QString(" Score: ") +
                 QString::number(score_));
}

void Score::setPlayerName(QString name)
{
    playerName_ = name;
}

QString Score::getPlayerName()
{
    return playerName_;
}

int Score::getScore()
{
    return score_;
}
