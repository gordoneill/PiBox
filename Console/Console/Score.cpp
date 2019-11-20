#include "Score.h"
#include <QFont>

Score::Score(QGraphicsItem * parent) :
    QGraphicsTextItem(parent),
    playerName_(""),
    score_(0),
    freezeScore_(false)
{
    setPlainText(playerName_ + QString(" Score: ") +
                 QString::number(score_));
    setDefaultTextColor(Qt::green);
    setFont(QFont("Calibri",16));
}

void Score::increase()
{
    if (!freezeScore_)
    {
        score_+=10;
        setPlainText(playerName_ + QString(" Score: ") +
                     QString::number(score_));
    }
}

void Score::decrease()
{
    if (!freezeScore_)
    {
        score_-=10;

        if (score_ < 0)
        {
            emit endGame();
        }
        else
        {
            setPlainText(playerName_ + QString(" Score: ") +
                         QString::number(score_));
        }
    }
}

void Score::setPlayerName(QString name)
{
    playerName_ = name;
    setPlainText(playerName_ + QString(" Score: ") +
                 QString::number(score_));
}

QString Score::getPlayerName()
{
    return playerName_;
}

int Score::getScore()
{
    return score_;
}

void Score::freezeScore()
{
    freezeScore_ = true;
}
