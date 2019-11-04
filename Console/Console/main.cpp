#include <QApplication>
#include <QGraphicsScene>
#include "spaceship.h"
#include "QGraphicsView"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QGraphicsScene * scene = new QGraphicsScene();
    Spaceship * spaceship = new Spaceship();
    spaceship->setRect(960,980,100,100);
    scene->addItem(spaceship);
    spaceship->setFlag(QGraphicsItem::ItemIsFocusable);
    spaceship->setFocus();

    QGraphicsView * view = new QGraphicsView(scene);
    view->setStyleSheet("background-color:black;");
    view->setFixedSize(1920, 1080);
    view->showFullScreen();
    return a.exec();
}
