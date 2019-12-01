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
    if (this->scene() == nullptr)
    {
        std::cerr << "Spaceship::keyPressEvent scene NULL" << std::endl;
    }
    else
    {
        switch(event->key())
        {
            case Qt::Key_Left:
                this->move(true);
                break;
            case Qt::Key_Right:
                this->move(false);
                break;
            case Qt::Key_Space:
                this->shoot();
                break;
            default:
                break;
        }
    }
}

void Spaceship::control(WMessage & msg)
{
    if (this->scene() == nullptr)
    {
        std::cerr << "Spaceship::keyPressEvent scene NULL" << std::endl;
    }
    else
    {
        switch(msg.type)
        {
            case eMsgTypes::BUTTON:
                this->shoot();
                break;
            case eMsgTypes::DIRECTION:
                if (msg.x_dir < 10)
                {
                    this->move(true);
                }
                else if (msg.x_dir > 10)
                {
                    this->move(false);
                }
                break;
            default:
            break;
        }
    }
}

void Spaceship::shoot()
{
    Laser * laser = new Laser();
    laser->setPos(
                x()+pixmap().width()/2-laser->pixmap().width()/2,
                y()-laser->pixmap().height());
    this->scene()->addItem(laser);
}

void Spaceship::move(bool direction)
{
    if (direction)
    {
        if (pos().x() > 0)
        {
            setPos(x()-10, y());
        }
    }
    else
    {
        if (pos().x() + pixmap().width() < this->scene()->width())
        {
            setPos(x()+10, y());
        }
    }
}
