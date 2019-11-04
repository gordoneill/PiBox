#include "laser.h"
#include <QTimer>

Laser::Laser()
{
    setRect(0,0,10,50);
    QTimer * timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this , SLOT(move()));
    timer->start(50);
}

void Laser::move()
{
    setPos(x(), y()-10);
}
