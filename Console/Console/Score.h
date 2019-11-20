#ifndef SCORE_H
#define SCORE_H

#include <QGraphicsTextItem>
#include <QString>

class Score : public QGraphicsTextItem
{
    Q_OBJECT
public:
    Score(QGraphicsItem * parent = nullptr);
    void increase();
    void decrease();
    void setPlayerName(QString name);
    QString getPlayerName();
    int getScore();
    void freezeScore();
signals:
    void endGame();
private:
    QString playerName_;
    int score_;
    bool freezeScore_;
};

#endif // SCORE_H
