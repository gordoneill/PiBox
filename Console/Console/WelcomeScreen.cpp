#include "WelcomeScreen.h"
#include <stdlib.h>
#include <unistd.h>

WelcomeScreen::WelcomeScreen(int h, int v, QWidget * /*parent*/) :
    scene_(),
    gameSelection_(eGames_T::NUM_GAMES)
{
    setBackgroundBrush(QBrush(QImage(":/graphics/welcome.jpg")));
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFixedSize(h, v);

    scene_ = new QGraphicsScene();
    scene_->setSceneRect(0, 0, h, v);
    setScene(scene_);

    QGraphicsTextItem * welcome = new QGraphicsTextItem();
    welcome->setDefaultTextColor(Qt::gray);
    welcome->setFont(QFont("Calibri",72));
    welcome->setPlainText("Welcome");
    welcome->setPos(scene_->sceneRect().width()/2 - welcome->boundingRect().width()/2,
                    scene_->sceneRect().height()/2 - welcome->boundingRect().height()/2 - 250);
    scene_->addItem(welcome);

    QGraphicsTextItem * select = new QGraphicsTextItem();
    select->setDefaultTextColor(Qt::gray);
    select->setFont(QFont("Calibri",30));
    select->setPlainText("Select a Game");
    select->setPos(scene_->sceneRect().width()/2 - select->boundingRect().width()/2,
                   scene_->sceneRect().height()/2 - select->boundingRect().height()/2 -170);
    scene_->addItem(select);

    for (int game = 0; game < eGames_T::NUM_GAMES; game++)
    {
        eGames_T gameType = (eGames_T) game;
        QGraphicsTextItem * gameSelect = new QGraphicsTextItem();
        gameSelect->setDefaultTextColor(Qt::white);
        gameSelect->setFont(QFont("Calibri",72));

        switch (gameType)
        {
            case eGames_T::ASTEROIDS:
                gameSelect->setPlainText("Asteroids");
                break;
            default:
                break;
        }

        gameSelect->setPos(scene_->sceneRect().width()/2 -
                               gameSelect->boundingRect().width()/2,
                           scene_->sceneRect().height()/2 -
                               gameSelect->boundingRect().height()/2 + 100);
        scene_->addItem(gameSelect);
    }
}

WelcomeScreen::~WelcomeScreen()
{
    delete scene_;
}

void WelcomeScreen::setConsoleStatus(ConsoleStatus * status)
{
    scene_->addItem(status->getConnectionPix());
    scene_->addItem(status->getBatteryPix());
}

void WelcomeScreen::keyPressEvent(QKeyEvent * /*event*/)
{
    gameSelection_ = eGames_T::ASTEROIDS;
    emit gameSelected();
}
