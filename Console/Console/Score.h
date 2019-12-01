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
    void setLevel(int level);
    void setScore(int score);
    int getScore();
    void freezeScore(bool freezeState);
signals:
    void endGame();
private:
    void updateDisplay();
    QString playerName_;
    int score_;
    int level_;
    bool freezeScore_;
};

#endif // SCORE_H
