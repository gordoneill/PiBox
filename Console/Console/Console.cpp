#include "Console.h"

Console::Console(QWidget * /*parent*/) :
    gameSelection_(eGames_T::NUM_GAMES),
    welcome_(nullptr),
    game_(nullptr),
    status_(nullptr),
    connectionTimer_(),
    welcomeTimer_(),
    controlTimer_(),
    introView_(),
    h_(0),
    v_(0)
{
    getDesktopResolution(h_, v_);
    welcome_ = new WelcomeScreen(h_, v_);
    status_ = new ConsoleStatus(h_, v_);
    welcome_->setConsoleStatus(status_);
    welcome_->setCursor(Qt::BlankCursor);
    connect(welcome_, SIGNAL(gameSelected()), this, SLOT(onGameSelected()));

    introView_ = new QGraphicsView;
    introView_->setStyleSheet("background-color:black;");
    introView_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    introView_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    introView_->setFixedSize(h_, v_);

    QGraphicsScene * scene = new QGraphicsScene;
    scene->setSceneRect(0, 0, h_, v_);
    introView_->setCursor(Qt::BlankCursor);
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

    connect(&controlTimer_, SIGNAL(timeout()), this, SLOT(control()));
    controlTimer_.start(1);
}

Console::~Console()
{
    delete welcome_;
    delete game_;
}

void Console::init()
{
    welcomeTimer_.stop();

    welcome_->showFullScreen();
    welcome_->setFocus();

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
        game_->setCursor(Qt::BlankCursor);
        game_->setConsoleStatus(status_);
        connect(game_, SIGNAL(backToWelcome()), this, SLOT(init()));
    }
    else
    {
        game_->reset();
    }

    game_->showFullScreen();
    game_->setFocus();

    welcome_->close();
}

void Console::addControl(WMessage msgIn)
{
    controlQueue.push(msgIn);
}

void Console::control()
{
    for (unsigned int i = 0; i < controlQueue.size(); i++)
    {
        WMessage msg = controlQueue.front();
        controlQueue.pop();
        switch(msg.type)
        {
            case eMsgTypes::STATUS:
                connectionTimer_.stop();
                connectionTimer_.start(2000);
                status_->updateConnection(true);
                status_->updateBattery(msg.battery);
                break;
            case eMsgTypes::BUTTON:
            case eMsgTypes::DIRECTION:
                if (game_ && game_->hasFocus())
                {
                    game_->control(msg);
                }
                else if (welcome_ && welcome_->hasFocus())
                {
                    welcome_->control(msg);
                }
                break;
            default:
                break;
        }
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
