#include "Laser.h"
#include "Asteroid.h"
#include "Score.h"
#include <iostream>

Laser::Laser(QGraphicsItem * parent) :
    QObject(),
    QGraphicsPixmapItem(parent)
{
    setPixmap(QPixmap(":/graphics/laser.png"));
    connect(&moveTimer_, SIGNAL(timeout()), this , SLOT(move()));
    moveTimer_.start(5);
}

void Laser::move()
{
    if (this->scene() == nullptr)
    {
        std::cerr << "Laser::move scene NULL" << std::endl;
    }
    else
    {
        for (auto * collision : collidingItems())
        {
            if (typeid(*collision) == typeid(Asteroid))
            {
                increaseScore();

                scene()->removeItem(collision);
                scene()->removeItem(this);

                delete collision;
                delete this;

                return;
            }
        }

        setPos(x(), y()-5);
        if (this->pos().y() < (this->pixmap().height() * -1))
        {
            this->scene()->removeItem(this);
            delete this;
        }
    }
}

void Laser::increaseScore()
{
    for (auto * item : scene()->items())
    {
        if (typeid(*item) == typeid(Score))
        {
            Score * gameScore = static_cast<Score *>(item);
            gameScore->increase();
        }
    }
}
