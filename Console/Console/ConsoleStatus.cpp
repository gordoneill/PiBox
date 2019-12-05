#include "ConsoleStatus.h"
#include <QFont>

ConsoleStatus::ConsoleStatus(int h, int /*v*/, QGraphicsItem * parent) :
    batteryLevel_(0),
    connected_(false),
    connectionPix_(nullptr),
    batteryTxt_(nullptr)
{
    connectionPix_ = new QGraphicsPixmapItem(parent);
    connectionPix_->setPixmap(QPixmap(":/graphics/disconnected.png"));
    connectionPix_->setPos(h - connectionPix_->pixmap().width() - 5, 0);

    batteryTxt_ = new QGraphicsTextItem(parent);
    batteryTxt_->setDefaultTextColor(Qt::yellow);
    batteryTxt_->setFont(QFont("Calibri",16));
    batteryTxt_->setPlainText("|????|");
    batteryTxt_->setPos(h-batteryTxt_->boundingRect().width() - 5,
                        connectionPix_->pixmap().height() + 5);
}

QGraphicsTextItem * ConsoleStatus::getBatteryPix()
{
    return batteryTxt_;
}

QGraphicsPixmapItem * ConsoleStatus::getConnectionPix()
{
    return connectionPix_;
}

void ConsoleStatus::updateBattery(unsigned int level)
{
    batteryLevel_ = level;
    updateDisplay();
}

void ConsoleStatus::updateConnection(bool status)
{
    connected_ = status;
    updateDisplay();
}

void ConsoleStatus::updateDisplay()
{
    if (batteryLevel_ < 10)
    {
        batteryTxt_->setPlainText("|----|");
        batteryTxt_->setDefaultTextColor(Qt::red);
    }
    else if (batteryLevel_ < 35)
    {
        batteryTxt_->setPlainText("|+---|");
        batteryTxt_->setDefaultTextColor(Qt::yellow);
    }
    else if (batteryLevel_ < 60)
    {
        batteryTxt_->setPlainText("|++--|");
        batteryTxt_->setDefaultTextColor(Qt::green);
    }
    else if (batteryLevel_ < 85)
    {
        batteryTxt_->setPlainText("|+++-|");
        batteryTxt_->setDefaultTextColor(Qt::green);
    }
    else
    {
        batteryTxt_->setPlainText("|++++|");
        batteryTxt_->setDefaultTextColor(Qt::green);
    }

    if (connected_)
    {
        connectionPix_->setPixmap(QPixmap(":/graphics/connected.png"));
    }
    else
    {
        connectionPix_->setPixmap(QPixmap(":/graphics/disconnected.png"));
        batteryTxt_->setPlainText("|????|");
        batteryTxt_->setDefaultTextColor(Qt::yellow);
    }
}
