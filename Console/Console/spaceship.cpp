#include "spaceship.h"

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
        case  Qt::Key_Right:
            setPos(x()+10, y());
            break;
        default:
            break;
    }
}
