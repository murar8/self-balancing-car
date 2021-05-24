#include "ControlLoop.h"
#include <Arduino.h>

ControlLoop::ControlLoop(PIDController &balance_loop, PIDController &velocity_loop)
    : balance_loop_(balance_loop), velocity_loop_(velocity_loop){};

bool ControlLoop::compute(float angle, float speed_l, float speed_r)
{
    float abs_angle = abs(angle);

    if (abs_angle > MAX_LEAN_ANGLE_RAD && started_)
    {
        balance_loop_.disable();
        velocity_loop_.disable();
        started_ = false;
        return true;
    }

    if (!started_)
    {
        if (abs_angle > STARTUP_ANGLE_RAD)
        {
            starting_ = false;
            return false;
        }

        if (!starting_)
        {
            starting_ = true;
            start_timestamp_ = millis();
            return false;
        }

        if (millis() - start_timestamp_ > STARTUP_TIME)
        {
            balance_loop_.enable();
            velocity_loop_.enable();
            starting_ = false;
            started_ = true;
        }
    }

    float speed_avg = (speed_l + speed_r) / 2;
    bool shouldUpdateSpeed = velocity_loop_.compute(speed_avg);
    bool shouldUpdateAngle = balance_loop_.compute(angle);

    return shouldUpdateAngle || shouldUpdateSpeed;
}
