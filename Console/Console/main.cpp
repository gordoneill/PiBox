#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "spaceship.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QGraphicsScene * scene = new QGraphicsScene();
    Spaceship * spaceship = new Spaceship();
    spaceship->setRect(0,0,100,100);
    scene->addItem(spaceship);
    spaceship->setFlag(QGraphicsItem::ItemIsFocusable);
    spaceship->setFocus();
    QGraphicsView * view = new QGraphicsView(scene);
    //view->setStyleSheet("background-color:black;");
    view->setFixedSize(1920, 1080);
    scene->setSceneRect(0,0,1920,1080);
    spaceship->setPos(view->width()/2-spaceship->rect().width()/2,
                      view->height()-spaceship->rect().height()-10);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->showFullScreen();
    return a.exec();
}
