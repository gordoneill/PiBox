#include "spaceship.h"
#include "laser.h"

Spaceship::Spaceship()
{

}

void Spaceship::keyPressEvent(QKeyEvent * event)
{
    switch(event->key())
    {
        case Qt::Key_Left:
            setPos(x()-10, y());
            break;
        case Qt::Key_Right:
            setPos(x()+10, y());
            break;
        case Qt::Key_Space:
        {
            Laser * laser = new Laser();
            laser->setPos(x()+rect().width()/2-laser->rect().width()/2, y());
            this->scene()->addItem(laser);
            break;
        }
        default:
            break;
    }
}
