#ifndef _BALANCE_LOOP_H_
#define _BALANCE_LOOP_H_

#include "PIDController.h"
#include "configuration.h"
#include <inttypes.h>
#include <math.h>

class BalanceLoop : public PIDController
{
  public:
    BalanceLoop();

    bool compute(float angle);

  private:
    static const constexpr float MAX_LEAN_ANGLE_RAD = (MAX_LEAN_ANGLE * M_PI) / 180.0;
    static const constexpr float STARTUP_ANGLE_RAD = (STARTUP_ANGLE * M_PI) / 180.0;

    bool starting_, started_ = false;
    uint32_t start_timestamp_;

    float duty_left_, duty_right_ = 0;
};

#endif // _BALANCE_LOOP_H_
