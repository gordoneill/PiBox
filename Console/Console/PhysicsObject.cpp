#include "PhysicsObject.h"

PhysicsObject::PhysicsObject()
{
    position_ = {0,0};
    velocity_ = {0,0};
    acceleration_ = {0,0};
    positionLimits_ = {0,0,0,0};
}

void PhysicsObject::setLimits(PositionLimits_T limits)
{
    positionLimits_ = limits;
}

void PhysicsObject::updateAcceleration(double x, double y)
{
    acceleration_.x += x;
    acceleration_.y += y;
}

void PhysicsObject::holdUp()
{
    velocity_ = {0,0};
    acceleration_ = {0,0};
}

void PhysicsObject::update()
{
    velocity_.x += acceleration_.x;
    velocity_.y += acceleration_.y;

    position_.x += velocity_.x;
    position_.y += velocity_.y;

    // Check Limits
    verifyLimits();

    //friction decay
    applyFriction();
}

void PhysicsObject::verifyLimits()
{
    if (position_.x > positionLimits_.right)
    {
        position_.x = positionLimits_.right;
        holdUp();
    }
    if (position_.x < positionLimits_.left)
    {
        position_.x = positionLimits_.left;
        holdUp();
    }
    // Qt makes y = 0 the top of the screen
    if (position_.y < positionLimits_.top)
    {
        position_.y = positionLimits_.top;
        holdUp();
    }
    if (position_.y > positionLimits_.bottom)
    {
        position_.y = positionLimits_.bottom;
        holdUp();
    }


    if (velocity_.x > VELOCITY_CAP)
    {
        velocity_.x = VELOCITY_CAP;
    }
    if (velocity_.x < (VELOCITY_CAP * -1))
    {
        velocity_.x = VELOCITY_CAP * -1;
    }
    if (velocity_.y > VELOCITY_CAP)
    {
        velocity_.y = VELOCITY_CAP;
    }
    if (velocity_.y < (VELOCITY_CAP * -1))
    {
        velocity_.y = VELOCITY_CAP * -1;
    }

    if (acceleration_.x > ACCEL_CAP)
    {
        acceleration_.x = ACCEL_CAP;
    }
    if (acceleration_.x < (ACCEL_CAP * -1))
    {
        acceleration_.x = ACCEL_CAP * -1;
    }
    if (acceleration_.y > ACCEL_CAP)
    {
        acceleration_.y = ACCEL_CAP;
    }
    if (acceleration_.y < (ACCEL_CAP * -1))
    {
        acceleration_.y = ACCEL_CAP * -1;
    }
}

void PhysicsObject::applyFriction()
{
    acceleration_.x /= ACCEL_FRIC;
    acceleration_.y /= ACCEL_FRIC;
    velocity_.x /= VELOCITY_FRIC;
    velocity_.y /= VELOCITY_FRIC;
}
