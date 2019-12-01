#ifndef SPACESHIP_H
#define SPACESHIP_H

#include <QKeyEvent>
#include <QGraphicsPixmapItem>
#include <QObject>
#include <QGraphicsItem>
#include "WMsg_structs.h"

class Spaceship : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Spaceship(QGraphicsItem * parent = nullptr);
    void control(WMessage & msg);
private:
    void keyPressEvent(QKeyEvent * event);
    void shoot();
    void move(bool direction);
};

#endif // SPACESHIP_H
