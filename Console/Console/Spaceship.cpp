#include "Spaceship.h"
#include "Laser.h"
#include <iostream>

Spaceship::Spaceship(QGraphicsItem * parent) :
    QGraphicsPixmapItem(parent)
{
    setPixmap(QPixmap(":/graphics/spaceship.png"));
}

void Spaceship::keyPressEvent(QKeyEvent * event)
{
    if (this->scene() == NULL)
    {
        std::cerr << "Spaceship::keyPressEvent scene NULL" << std::endl;
    }
    else
    {
        switch(event->key())
        {
            case Qt::Key_Left:
                if (pos().x() > 0)
                {
                    setPos(x()-10, y());
                }
                break;
            case Qt::Key_Right:
                if (pos().x() + pixmap().width() < this->scene()->width())
                {
                    setPos(x()+10, y());
                }
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
}
