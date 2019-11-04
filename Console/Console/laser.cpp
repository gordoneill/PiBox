#include "laser.h"

Laser::Laser()
{
    setRect(0,0,10,50);
    connect(&moveTimer_, SIGNAL(timeout()), this , SLOT(move()));
    moveTimer_.start(50);
}

void Laser::move()
{
    setPos(x(), y()-10);
    if (this->pos().y() + this->rect().height() < 0)
    {
        this->scene()->removeItem(this);
        delete this;
    }
}
