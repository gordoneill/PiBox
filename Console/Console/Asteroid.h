#ifndef ASTEROID_H
#define ASTEROID_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QGraphicsItem>
#include <QTimer>

class Asteroid : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Asteroid(QGraphicsItem * parent = 0);
signals:
    void endGame();
private slots:
    void move();
private:
    void decreaseScore();
    QTimer moveTimer_;
};

#endif // ASTEROID_H
