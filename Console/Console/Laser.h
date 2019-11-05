#ifndef LASER_H
#define LASER_H

#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QObject>
#include <QTimer>

class Laser : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Laser(QGraphicsItem * parent=0);
private slots:
    void move();
private:
    QTimer moveTimer_;
};

#endif // LASER_H
