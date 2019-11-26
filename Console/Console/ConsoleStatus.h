#ifndef CONSOLESTATUS_H
#define CONSOLESTATUS_H

#include <QGraphicsPixmapItem>

class ConsoleStatus
{
public:
    ConsoleStatus(int h, int v, QGraphicsItem * parent = nullptr);
    void updateBattery(unsigned int level);
    void updateConnection(bool status);
    QGraphicsTextItem * getBatteryPix();
    QGraphicsPixmapItem * getConnectionPix();
private:
    void updateDisplay();
    unsigned int batteryLevel_;
    bool connected_;
    QGraphicsPixmapItem * connectionPix_;
    QGraphicsTextItem * batteryTxt_;
};

#endif // CONSOLESTATUS_H
