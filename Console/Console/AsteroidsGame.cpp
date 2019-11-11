#include "AsteroidsGame.h"
#include "Asteroid.h"
#include <QBrush>
#include <QImage>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>

AsteroidsGame::AsteroidsGame(QWidget * /*parent*/)
{
    int h, v;
    getDesktopResolution(h, v);

    scene_ = new QGraphicsScene();
    scene_->setSceneRect(0, 0, h, v);
    setScene(scene_);

    setBackgroundBrush(QBrush(QImage(":/graphics/asteroidsWallpaper.jpg")));
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFixedSize(h, v);

    spaceship_ = new Spaceship();
    spaceship_->setFlag(QGraphicsItem::ItemIsFocusable);
    spaceship_->setPos(this->width()/2-spaceship_->pixmap().width()/2,
                       this->height()-spaceship_->pixmap().height()-10);
    spaceship_->setFocus();
    scene_->addItem(spaceship_);

    score_ = new Score();
    score_->setPlayerName("Gordon");
    scene_->addItem(score_);

    // Spawn Asteroids
    connect(&spawnTimer_, SIGNAL(timeout()), this, SLOT(spawn()));
    spawnTimer_.start(2000);
}

AsteroidsGame::~AsteroidsGame()
{
    delete scene_;
    delete spaceship_;
    delete score_;
}

void AsteroidsGame::spawn()
{
    Asteroid * asteroid = new Asteroid();
    int asteroidStart = rand() %
            ((int)scene_->sceneRect().width() - asteroid->pixmap().width()*2)
                + asteroid->pixmap().width();
    asteroid->setPos(asteroidStart, 0);
    scene_->addItem(asteroid);
    connect(asteroid, SIGNAL(endGame()), this, SLOT(endGame()));
}

void AsteroidsGame::getDesktopResolution(int & h, int & v)
{
    Display * d = XOpenDisplay(NULL);
    Screen * s = DefaultScreenOfDisplay(d);
    v = s->height;
    h = s->width;
}

void AsteroidsGame::endGame()
{
    // save off score to file
    // somehow get the game back to game selection
    spawnTimer_.stop();
    scene_->removeItem(spaceship_);

    QGraphicsTextItem * gameOver = new QGraphicsTextItem();
    gameOver->setPlainText("GAME OVER");
    gameOver->setDefaultTextColor(Qt::red);
    gameOver->setFont(QFont("Calibri",72));
    gameOver->setPos(scene_->sceneRect().width()/2 -
                        gameOver->boundingRect().width()/2,
                     scene_->sceneRect().height()/2 -
                        gameOver->boundingRect().height()/2);
    scene_->addItem(gameOver);

    connect(&gameOverTimer_, SIGNAL(timeout()), this, SLOT(close()));
    gameOverTimer_.start(5000);
}
