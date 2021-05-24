#include "PIDController.h"

PIDController::PIDController(float sample_period, float output_min, float output_max, bool direct)
    : PID((double *)&input_, (double *)&output_, (double *)&setpoint_, 0, 0, 0, direct ? DIRECT : REVERSE)
{
    SetOutputLimits(output_min, output_max);
    SetSampleTime(sample_period);
};
