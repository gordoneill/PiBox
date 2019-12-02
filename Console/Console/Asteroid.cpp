#include "Asteroid.h"
#include "Spaceship.h"
#include <QGraphicsScene>
#include <iostream>
#include <Score.h>

Asteroid::Asteroid(QGraphicsItem * parent) :
    QObject(),
    QGraphicsPixmapItem(parent),
    moveTimer_()
{
    setPixmap(QPixmap(":/graphics/asteroid.png"));
    connect(&moveTimer_, SIGNAL(timeout()), this, SLOT(move()));
    moveTimer_.start(5);
}

void Asteroid::move()
{
    if (this->scene() == nullptr)
    {
        std::cerr << "Asteroid::move scene NULL" << std::endl;
    }
    else
    {
        setPos(x(),y()+1);

        // if a colliding items is the spaceship, end game
        for (auto * item : collidingItems())
        {
            if (typeid(*item) == typeid(Spaceship))
            {
                setPixmap(QPixmap(":/graphics/explosion.png"));
                emit endGame();
            }
        }

        if (pos().y() > this->scene()->height())
        {
            // decrease the score
            decreaseScore();

            scene()->removeItem(this);
            delete this;
        }
    }
}

void Asteroid::decreaseScore()
{
    for (auto * item : scene()->items())
    {
        if (typeid(*item) == typeid(Score))
        {
            Score * gameScore = static_cast<Score *>(item);
            gameScore->decrease();
        }
    }
}
