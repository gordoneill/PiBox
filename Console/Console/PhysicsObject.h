#ifndef PHYSICSOBJECT_H
#define PHYSICSOBJECT_H

#include <math.h>
#include <tuple>

#define ACCEL_FRIC 1.03
#define VELOCITY_FRIC 1.02
#define ACCEL_CAP 0.3
#define VELOCITY_CAP 12

struct PhysicsComponent_T {
    double x;
    double y;
};

struct PositionLimits_T {
    double left;
    double right;
    double top;
    double bottom;
};

class PhysicsObject
{
public:
    PhysicsObject();
    void updateAcceleration(double x, double y);
    void setLimits(PositionLimits_T limits);
    void holdUp();
protected:
    void update();
    void verifyLimits();
    void applyFriction();

    PhysicsComponent_T position_;
    PhysicsComponent_T velocity_;
    PhysicsComponent_T acceleration_;
    PositionLimits_T positionLimits_;
};

#endif // PHYSICSOBJECT_H
