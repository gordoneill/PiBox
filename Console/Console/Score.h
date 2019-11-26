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
    void setLevel(int level);
    QString getPlayerName();
    int getScore();
    void freezeScore();
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
