#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setStyleSheet("background-color:black;");
    QPixmap pixmap("/home/pi/Git/PiBox/Console/pi.png");
    ui->PiLogo->setPixmap(pixmap);
}

MainWindow::~MainWindow()
{
    delete ui;
}
