#include "Console.h"

Console::Console(QWidget * /*parent*/) :
    gameSelection_(eGames_T::NUM_GAMES),
    welcome_(nullptr),
    game_(nullptr),
    status_(nullptr),
    connectionTimer_(),
    welcomeTimer_(),
    introView_(),
    h_(0),
    v_(0)
{
    getDesktopResolution(h_, v_);
    welcome_ = new WelcomeScreen(h_, v_);
    status_ = new ConsoleStatus(h_, v_);

    introView_ = new QGraphicsView;
    introView_->setStyleSheet("background-color:black;");
    introView_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    introView_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    introView_->setFixedSize(h_, v_);

    QGraphicsScene * scene = new QGraphicsScene;
    scene->setSceneRect(0, 0, h_, v_);
    introView_->setScene(scene);

    QGraphicsPixmapItem * pi = new QGraphicsPixmapItem();
    pi->setPixmap(QPixmap(":/graphics/pi.png"));
    pi->setPos(scene->sceneRect().width()/2 -
                  pi->pixmap().width()/2,
               scene->sceneRect().height()/2 -
                  pi->pixmap().height()/2);
    scene->addItem(pi);

    introView_->showFullScreen();

    connect(&welcomeTimer_, SIGNAL(timeout()), this, SLOT(init()));
    welcomeTimer_.start(5000);

    connect(&connectionTimer_, SIGNAL(timeout()), this, SLOT(connectionTimeout()));
    connectionTimer_.start(2000);
}

Console::~Console()
{
    delete welcome_;
    delete game_;
}

void Console::init()
{
    welcomeTimer_.stop();

    welcome_->setConsoleStatus(status_);
    welcome_->showFullScreen();
    welcome_->setFocus();

    connect(welcome_, SIGNAL(gameSelected()), this, SLOT(onGameSelected()));

    introView_->close();
    if (game_ != nullptr)
    {
        game_->close();
    }
}

void Console::onGameSelected()
{
    if (game_ == nullptr)
    {
        game_ = new AsteroidsGame(h_, v_);
    }
    else
    {
        exit(0);
    }

    game_->setConsoleStatus(status_);
    game_->showFullScreen();
    game_->setFocus();

    connect(game_, SIGNAL(backToWelcome()), this, SLOT(init()));
    welcome_->close();
}

void Console::control(WMessage msgIn)
{
    switch(msgIn.type)
    {
        case eMsgTypes::STATUS:
            connectionTimer_.stop();
            connectionTimer_.start(2000);
            status_->updateConnection(true);
            status_->updateBattery(msgIn.battery);
            break;
        case eMsgTypes::BUTTON:
        case eMsgTypes::DIRECTION:
            if (game_)
            {
                game_->control(msgIn);
            }
            break;
        default:
            break;
    }
}

void Console::connectionTimeout()
{
    status_->updateConnection(false);
}

void Console::getDesktopResolution(int & h, int & v)
{
    Display * d = XOpenDisplay(nullptr);
    Screen * s = DefaultScreenOfDisplay(d);
    v = s->height;
    h = s->width;
}
