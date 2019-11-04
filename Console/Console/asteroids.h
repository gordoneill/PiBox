#ifndef ASTEROIDS_H
#define ASTEROIDS_H
#include <QGraphicsView>
#include <QWidget>
#include <QGraphicsScene>
#include "spaceship.h"

class Asteroids : public QGraphicsView
{
public:
    Asteroids(QWidget * parent=0);
private:
    QGraphicsScene * scene_;
    Spaceship * spaceship_;
};

#endif // ASTEROIDS_H
