#include "ConsoleStatus.h"
#include <QFont>

ConsoleStatus::ConsoleStatus(int h, int /*v*/, QGraphicsItem * parent) :
    batteryLevel_(0),
    connected_(false),
    connectionPix_(nullptr),
    batteryPix_(nullptr)
{
    connectionPix_ = new QGraphicsPixmapItem(parent);
    connectionPix_->setPixmap(QPixmap(":/graphics/disconnected.png"));
    connectionPix_->setPos(h - connectionPix_->pixmap().width() - 5, 0);

    batteryPix_ = new QGraphicsPixmapItem(parent);
    batteryPix_->setPixmap(QPixmap(":/graphics/empty.png"));
    batteryPix_->setPos(connectionPix_->pos().x() -
                        batteryPix_->pixmap().width() - 5, 0);
}

QGraphicsPixmapItem * ConsoleStatus::getBatteryPix()
{
    return batteryPix_;
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
        batteryPix_->setPixmap(QPixmap(":/graphics/batt0.png"));
    }
    else if (batteryLevel_ < 35)
    {
        batteryPix_->setPixmap(QPixmap(":/graphics/batt1.png"));
    }
    else if (batteryLevel_ < 60)
    {
        batteryPix_->setPixmap(QPixmap(":/graphics/batt2.png"));
    }
    else if (batteryLevel_ < 85)
    {
        batteryPix_->setPixmap(QPixmap(":/graphics/batt3.png"));
    }
    else
    {
        batteryPix_->setPixmap(QPixmap(":/graphics/batt4.png"));
    }

    if (connected_)
    {
        connectionPix_->setPixmap(QPixmap(":/graphics/disconnected.png"));
    }
    else
    {
        connectionPix_->setPixmap(QPixmap(":/graphics/connected.png"));
    }
}
