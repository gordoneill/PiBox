#ifndef ASTEROIDS_H
#define ASTEROIDS_H

#include <QGraphicsView>
#include <QWidget>
#include <QTimer>
#include <QGraphicsScene>
#include "Spaceship.h"
#include "Score.h"
#include <X11/Xlib.h>

class AsteroidsGame : public QGraphicsView
{
public:
    AsteroidsGame(QWidget * parent=0);
    ~AsteroidsGame();
public slots:
    void spawnAsteroid();
    void endGame();
private:
    void getDesktopResolution(int& h, int& v);
    QGraphicsScene * scene_;
    Spaceship * spaceship_;
    QTimer spawnTimer_;
    Score * score_;
};

#endif // ASTEROIDS_H
