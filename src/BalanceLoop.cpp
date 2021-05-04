#include "BalanceLoop.h"
#include <Arduino.h>

BalanceLoop::BalanceLoop()
    : PIDController(BALANCE_PID_SAMPLE_PERIOD, static_cast<double>(INT8_MIN), static_cast<double>(INT8_MAX)){};

bool BalanceLoop::compute(float angle)
{
    float abs_angle = abs(angle);

    if (abs_angle > MAX_LEAN_ANGLE_RAD && started_)
    {
        disable();
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
            enable();
            starting_ = false;
            started_ = true;
        }
    }

    return PIDController::compute(angle);
}
