#include "Asteroid.h"
#include "Spaceship.h"
#include <QTimer>
#include <QGraphicsScene>
#include <QList>
#include <stdlib.h>
#include <iostream>
#include <Score.h>

Asteroid::Asteroid(qreal sceneWidth, QGraphicsItem * parent) :
    QObject(),
    QGraphicsPixmapItem(parent)
{
    setPixmap(QPixmap(":/graphics/asteroid.png"));
    int asteroidWidth = pixmap().width();
    std::cout << asteroidWidth << std::endl;
    int asteroidStart =
            rand() % ((int)sceneWidth - asteroidWidth*2) + asteroidWidth;
    setPos(asteroidStart, 0);

    connect(&moveTimer_, SIGNAL(timeout()), this, SLOT(move()));
    moveTimer_.start(25);
}

void Asteroid::move()
{
    if (this->scene() == NULL)
    {
        std::cerr << "Asteroid::move scene NULL" << std::endl;
    }
    else
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
            Score * gameScore = (Score *) item;
            gameScore->decrease();
        }
    }
}
