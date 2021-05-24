#ifndef _CONTROL_LOOP_H_
#define _CONTROL_LOOP_H_

#include "PIDController.h"
#include "configuration.h"
#include <inttypes.h>
#include <math.h>

class ControlLoop
{
  public:
    ControlLoop(PIDController &balance_loop, PIDController &velocity_loop);

    /// Perform the calculations necessary to update the output.
    bool compute(float angle, float speed_l, float speed_r);

    float getDutyLeft();
    float getDutyRight();

  private:
    static const constexpr float MAX_LEAN_ANGLE_RAD = (MAX_LEAN_ANGLE * M_PI) / 180.0;
    static const constexpr float STARTUP_ANGLE_RAD = (STARTUP_ANGLE * M_PI) / 180.0;
    static const constexpr float MAX_WORKING_ANGLE_RAD = (MAX_WORKING_ANGLE * M_PI) / 180.0;

    PIDController &balance_loop_;
    PIDController &velocity_loop_;

    bool starting_, started_ = false;
    uint32_t start_timestamp_;
};

#endif // _CONTROL_LOOP_H_
