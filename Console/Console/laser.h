#ifndef LASER_H
#define LASER_H
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QTimer>

class Laser : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    Laser();
public slots:
    void move();
private:
    QTimer moveTimer_;
};

#endif // LASER_H
