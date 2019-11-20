#ifndef WELCOMESCREEN_H
#define WELCOMESCREEN_H

#include <QGraphicsView>
#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include "ConsoleStatus.h"

enum eGames_T {
    ASTEROIDS,
    NUM_GAMES
};

class WelcomeScreen : public QGraphicsView
{
    Q_OBJECT
public:
    WelcomeScreen(int h, int v, QWidget * parent = nullptr);
    ~WelcomeScreen();
    void setConsoleStatus(ConsoleStatus * status);
signals:
    void gameSelected();
private:
    void getDesktopResolution(int& h, int& v);
    void keyPressEvent(QKeyEvent * event);

    QGraphicsScene * scene_;
    eGames_T gameSelection_;
};

#endif // WELCOMESCREEN_H
