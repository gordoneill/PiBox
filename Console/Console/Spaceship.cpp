#include "Spaceship.h"
#include "Laser.h"
#include <iostream>

Spaceship::Spaceship(int h, int v, QGraphicsItem * parent) :
    QGraphicsPixmapItem(parent)
{
    setPixmap(QPixmap(":/graphics/spaceship.png"));

    PositionLimits_T limits;
    limits.top = 0;
    limits.bottom = v - pixmap().height();
    limits.left = 0;
    limits.right = h - pixmap().width();
    setLimits(limits);

    connect(&updateTimer_, SIGNAL(timeout()), this, SLOT(redraw()));
    updateTimer_.start(10);
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
                this->updateAcceleration(controlToAcceleration(0), 0);
                break;
            case Qt::Key_Right:
                this->updateAcceleration(controlToAcceleration(100), 0);
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
                this->updateAcceleration(controlToAcceleration(msg.x_dir), 0);
                break;
            default:
            break;
        }
    }
}

void Spaceship::setPos(qreal x, qreal y)
{
    position_ = {x, y};
    QGraphicsPixmapItem::setPos(x, y);
}

void Spaceship::freeze(bool freezeState)
{
    if (freezeState)
    {
        updateTimer_.stop();
    }
    else
    {
        updateTimer_.start(10);
    }
}

void Spaceship::redraw()
{
    PhysicsObject::update();
    QGraphicsPixmapItem::setPos(position_.x, position_.y);
}

void Spaceship::shoot()
{
    Laser * laser = new Laser();
    laser->setPos(x()+pixmap().width()/2-laser->pixmap().width()/2,
                  y()-laser->pixmap().height());
    this->scene()->addItem(laser);
}

double Spaceship::controlToAcceleration(uint32_t control)
{
    double accelCtrl = static_cast<double>(control);
    accelCtrl -= 50;
    accelCtrl /= 250;
    return accelCtrl;
}
