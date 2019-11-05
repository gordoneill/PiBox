#include "AsteroidsGame.h"
#include "Asteroid.h"
#include <QBrush>
#include <QImage>

AsteroidsGame::AsteroidsGame(QWidget * /*parent*/)
{
    int h, v;
    getDesktopResolution(h, v);

    scene_ = new QGraphicsScene();
    scene_->setSceneRect(0, 0, h, v);
    setScene(scene_);

    setBackgroundBrush(QBrush(QImage(":/graphics/asteroidsWallpaper.png")));
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
    QObject::connect(&spawnTimer_, SIGNAL(timeout()), this, SLOT(spawnAsteroid()));
    spawnTimer_.start(2000);

    showFullScreen();
}

AsteroidsGame::~AsteroidsGame()
{
    delete scene_;
    delete spaceship_;
    delete score_;
}

void AsteroidsGame::spawnAsteroid()
{
    Asteroid * asteroid = new Asteroid();
    scene_->addItem(asteroid);
    connect(asteroid, SIGNAL(endGame()), this, SLOT(endGame()));
}

void AsteroidsGame::getDesktopResolution(int & h, int & v)
{
    Display * d = XOpenDisplay(NULL);
    Screen * s = DefaultScreenOfDisplay(d);
    h = s->height;
    v = s->width;
}

void AsteroidsGame::endGame()
{
    // save off score to file
    // somehow get the game back to game selection
    this->close();
}
