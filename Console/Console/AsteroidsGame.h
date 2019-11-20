#ifndef ASTEROIDS_H
#define ASTEROIDS_H

#include <QGraphicsView>
#include <QWidget>
#include <QTimer>
#include <QGraphicsScene>
#include "Spaceship.h"
#include "ConsoleStatus.h"
#include "Score.h"
#include <X11/Xlib.h>

#define MAX_LEVEL 5

class AsteroidsGame : public QGraphicsView
{
    Q_OBJECT
public:
    AsteroidsGame(QWidget * parent = nullptr);
    ~AsteroidsGame();
    void setConsoleStatus(ConsoleStatus * status);
signals:
    void backToWelcome();
private slots:
    void spawn();
    void increaseLevel();
    void endGame();
    void goBackToWelcome();
private:
    void getDesktopResolution(int& h, int& v);
    QGraphicsScene * scene_;
    Spaceship * spaceship_;
    QTimer spawnTimer_;
    QTimer levelTimer_;
    QTimer gameOverTimer_;
    Score * score_;
    int level_;
};

#endif // ASTEROIDS_H
