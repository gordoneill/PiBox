#ifndef SCORE_H
#define SCORE_H

#include <QGraphicsTextItem>
#include <QString>

class Score : public QGraphicsTextItem
{
public:
    Score(QGraphicsItem * parent = 0);
    void increase();
    void decrease();
    void setPlayerName(QString name);
    QString getPlayerName();
    int getScore();
private:
    QString playerName_;
    int score_;
};

#endif // SCORE_H
