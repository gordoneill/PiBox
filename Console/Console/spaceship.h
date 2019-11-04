#ifndef SPACESHIP_H
#define SPACESHIP_H
#include <QGraphicsRectItem>
#include <QKeyEvent>

class Spaceship : public QGraphicsRectItem
{
public:
    Spaceship();
    void keyPressEvent(QKeyEvent * event);
};

#endif // SPACESHIP_H
