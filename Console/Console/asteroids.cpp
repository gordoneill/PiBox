#include "asteroids.h"

Asteroids::Asteroids(QWidget * parent)
{
    scene_ = new QGraphicsScene();
    scene_->setSceneRect(0,0,1920,1080);
    setScene(scene_);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFixedSize(1920, 1080);
    //setStyleSheet("background-color:black;");

    spaceship_ = new Spaceship();
    spaceship_->setRect(0,0,100,100);
    spaceship_->setFlag(QGraphicsItem::ItemIsFocusable);
    spaceship_->setPos(this->width()/2-spaceship_->rect().width()/2,
                      this->height()-spaceship_->rect().height()-10);
    spaceship_->setFocus();
    scene_->addItem(spaceship_);

    showFullScreen();
}
