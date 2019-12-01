#include "Score.h"
#include <QFont>

Score::Score(QGraphicsItem * parent) :
    QGraphicsTextItem(parent),
    playerName_(""),
    score_(0),
    level_(1),
    freezeScore_(false)
{
    setDefaultTextColor(Qt::green);
    setFont(QFont("Calibri",16));
    updateDisplay();
}

void Score::increase()
{
    if (!freezeScore_)
    {
        score_+=10;
        updateDisplay();
    }
}

void Score::decrease()
{
    if (!freezeScore_)
    {
        score_-=10;

        if (score_ < -50)
        {
            emit endGame();
        }
        else
        {
            updateDisplay();
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

void Score::setLevel(int level)
{
    level_ = level;
    updateDisplay();
}

void Score::setScore(int score)
{
    score_ = score;
}

int Score::getScore()
{
    return score_;
}

void Score::freezeScore(bool freezeState)
{
    freezeScore_ = freezeState;
}

void Score::updateDisplay()
{
    setPlainText(playerName_ + QString(" Score: ") +
                 QString::number(score_) + "\n" +
                 QString("Level: ") + QString::number(level_));
}
