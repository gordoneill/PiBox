#include "Laser.h"
#include "Asteroid.h"
#include "Score.h"

Laser::Laser(QGraphicsItem * parent) :
    QObject(),
    QGraphicsPixmapItem(parent)
{
    setPixmap(QPixmap(":/graphics/laser.png"));
    connect(&moveTimer_, SIGNAL(timeout()), this , SLOT(move()));
    moveTimer_.start(50);
}

void Laser::move()
{
    // if one of the colliding items is an Asteroid,
    //    destroy both the Asteroid and the Laser
    for (auto * collision : collidingItems())
    {
        if (typeid(*collision) == typeid(Asteroid))
        {
            // increase the score
            for (auto * item : scene()->items())
            {
                if (typeid(*item) == typeid(Score))
                {
                    Score * gameScore = (Score *) item;
                    gameScore->increase();
                }
            }

            // remove them from the scene (still on the heap)
            scene()->removeItem(collision);
            scene()->removeItem(this);

            // delete them from the heap to save memory
            delete collision;
            delete this;

            return;
        }
    }

    setPos(x(), y()-10);
    if (this->pos().y() < 0)
    {
        this->scene()->removeItem(this);
        delete this;
    }
}
