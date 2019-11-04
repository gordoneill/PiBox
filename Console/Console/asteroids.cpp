#include "asteroids.h"

Asteroids::Asteroids(QWidget * /*parent*/)
{
    QGraphicsScene scene;
    scene.setSceneRect(0,0,1920,1080);
    setScene(&scene);

    setFixedSize(1920, 1080);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //setStyleSheet("background-color:black;");

    spaceship_.setRect(0,0,100,100);
    spaceship_.setFlag(QGraphicsItem::ItemIsFocusable);
    spaceship_.setPos(this->width()/2-spaceship_.rect().width()/2,
                      this->height()-spaceship_.rect().height()-10);
    spaceship_.setFocus();
    scene.addItem(&spaceship_);

    showFullScreen();
}
