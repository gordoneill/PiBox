#include "AsteroidsGame.h"
#include "Asteroid.h"
#include <QBrush>
#include <QImage>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>

AsteroidsGame::AsteroidsGame(int h, int v, QWidget * /*parent*/) :
    scene_(nullptr),
    spaceship_(nullptr),
    spawnTimer_(),
    gameOverTimer_(),
    score_(nullptr),
    level_(1)
{
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
    score_->setPlayerName("Player");
    score_->setLevel(level_);
    scene_->addItem(score_);
    connect(score_, SIGNAL(endGame()), this, SLOT(endGame()));

    // Spawn Asteroids
    connect(&levelTimer_, SIGNAL(timeout()), this, SLOT(increaseLevel()));
    levelTimer_.start(20000);

    // Spawn Asteroids
    connect(&spawnTimer_, SIGNAL(timeout()), this, SLOT(spawn()));
    spawnTimer_.start(4000);
}

AsteroidsGame::~AsteroidsGame()
{
    delete scene_;
    delete spaceship_;
    delete score_;
}

void AsteroidsGame::setConsoleStatus(ConsoleStatus * status)
{
    scene_->addItem(status->getConnectionPix());
    scene_->addItem(status->getBatteryPix());
}

void AsteroidsGame::control(WMessage msg)
{
    spaceship_->control(msg);
}

void AsteroidsGame::spawn()
{
    Asteroid * asteroid = new Asteroid();
    srand(static_cast<unsigned int>(clock()));
    int asteroidStart = rand() %
            (static_cast<int>(scene_->sceneRect().width()) -
                asteroid->pixmap().width()*2) + asteroid->pixmap().width();
    asteroid->setPos(asteroidStart, 0);
    scene_->addItem(asteroid);
    connect(asteroid, SIGNAL(endGame()), this, SLOT(endGame()));
}

void AsteroidsGame::increaseLevel()
{
    if (level_ < MAX_LEVEL)
    {
        level_++;
        score_->setLevel(level_);
        spawnTimer_.stop();
        switch (level_)
        {
            case 1:
                spawnTimer_.start(4000);
                break;
            case 2:
                spawnTimer_.start(3000);
                break;
            case 3:
                spawnTimer_.start(2000);
                break;
            case 4:
                spawnTimer_.start(1500);
                break;
            case 5:
                spawnTimer_.start(1100);
                break;
            default:
                break;
        }
    }
}

void AsteroidsGame::endGame()
{
    // save off score to file
    // somehow get the game back to game selection
    spawnTimer_.stop();
    scene_->removeItem(spaceship_);
    score_->freezeScore();

    QGraphicsTextItem * gameOver = new QGraphicsTextItem();
    gameOver->setPlainText("GAME OVER");
    gameOver->setDefaultTextColor(Qt::red);
    gameOver->setFont(QFont("Calibri",72));
    gameOver->setPos(scene_->sceneRect().width()/2 -
                        gameOver->boundingRect().width()/2,
                     scene_->sceneRect().height()/2 -
                        gameOver->boundingRect().height()/2);
    scene_->addItem(gameOver);

    connect(&gameOverTimer_, SIGNAL(timeout()), this, SLOT(goBackToWelcome()));
    gameOverTimer_.start(5000);
}

void AsteroidsGame::goBackToWelcome()
{
    emit backToWelcome();
}
