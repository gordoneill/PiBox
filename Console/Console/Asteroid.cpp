#include "Asteroid.h"
#include "Spaceship.h"
#include <QTimer>
#include <QGraphicsScene>
#include <QList>
#include <stdlib.h>

Asteroid::Asteroid(QGraphicsItem * parent) :
    QObject(),
    QGraphicsPixmapItem(parent)
{
    int random_number = rand() % (int) this->scene()->width();
    setPos(random_number, 0);

    setPixmap(QPixmap(":/graphics/asteroid.png"));

    connect(&moveTimer_, SIGNAL(timeout()), this, SLOT(move()));
    moveTimer_.start(50);
}

void Asteroid::move()
{
    setPos(x(),y()+5);

    // if a colliding items is the spaceship, end game
    for (auto * item : collidingItems())
    {
        if (typeid(*item) == typeid(Spaceship))
        {
            emit endGame();
        }
    }

    if (pos().y() > this->scene()->height())
    {
        scene()->removeItem(this);
        delete this;
    }
}
