#ifndef SPACESHIP_H
#define SPACESHIP_H

#include <QKeyEvent>
#include <QGraphicsPixmapItem>
#include <QObject>
#include <QGraphicsItem>

class Spaceship : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Spaceship(QGraphicsItem * parent = nullptr);
private:
    void keyPressEvent(QKeyEvent * event);
};

#endif // SPACESHIP_H
