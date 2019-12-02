#ifndef SPACESHIP_H
#define SPACESHIP_H

#include <QKeyEvent>
#include <QGraphicsPixmapItem>
#include <QObject>
#include <QGraphicsItem>
#include "WMsg_structs.h"
#include "PhysicsObject.h"
#include "QTimer"

class Spaceship : public QObject, public QGraphicsPixmapItem, public PhysicsObject
{
    Q_OBJECT
public:
    Spaceship(int h, int v, QGraphicsItem * parent = nullptr);
    void control(WMessage & msg);
    void setPos(qreal x, qreal y);
    void freeze(bool freezeState);
private slots:
    void redraw();
private:
    void keyPressEvent(QKeyEvent * event);
    void shoot();
    double controlToAcceleration(uint32_t control);
    QTimer updateTimer_;
};

#endif // SPACESHIP_H
