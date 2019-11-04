#ifndef LASER_H
#define LASER_H
#include <QGraphicsRectItem>
#include <QGraphicsScene>

class Laser : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    Laser();
public slots:
    void move();
};

#endif // LASER_H
