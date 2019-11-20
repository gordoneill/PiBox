#ifndef CONSOLE_H
#define CONSOLE_H

#include <QWidget>
#include <QTimer>
#include <QCoreApplication>
#include "AsteroidsGame.h"
#include "WelcomeScreen.h"
#include "ConsoleStatus.h"
#include "WMsg_structs.h"
#include <X11/Xlib.h>

class Console : public QObject
{
    Q_OBJECT
public:
    Console(QWidget * parent = nullptr);
    ~Console();
    void control(WMessage msgIn);
private slots:
    void init();
    void onGameSelected();
    void connectionTimeout();
private:
    void getDesktopResolution(int& h, int& v);
    void keyPressEvent(QKeyEvent * event);

    QGraphicsScene * scene_;
    eGames_T gameSelection_;
    WelcomeScreen * welcome_;
    AsteroidsGame * game_;
    ConsoleStatus * status_;
    QTimer connectionTimer_;
    QTimer welcomeTimer_;
    QGraphicsView * introView_;
    int h_, v_;
};

#endif // CONSOLE_H
