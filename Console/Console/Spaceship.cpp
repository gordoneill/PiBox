#include "Spaceship.h"
#include "Laser.h"

Spaceship::Spaceship(QGraphicsItem * parent) :
    QGraphicsPixmapItem(parent)
{
    setPixmap(QPixmap(":/graphics/spaceship.png"));
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
            laser->setPos(x()+pixmap().width()/2-laser->pixmap().width()/2, y());
            this->scene()->addItem(laser);
            break;
        }
        default:
            break;
    }
}
