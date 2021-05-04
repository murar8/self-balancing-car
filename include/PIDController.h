#ifndef _PID_CONTROLLER_H_
#define _PID_CONTROLLER_H_

#include <PID_v1.h>

class PIDController : PID
{
  public:
    PIDController(float sample_period, float output_min, float output_max, bool direct = true);

    /// Performs the PID calculation. It should be called every time loop() cycles.
    inline bool compute(float input)
    {
        input_ = input;
        return PID::Compute();
    }

    /// Enable the PID loop.
    inline void enable()
    {
        SetMode(AUTOMATIC);
    }

    /// Disable the PID loop.
    inline void disable()
    {
        output_ = 0;
        SetMode(MANUAL);
    }

    /// Get the last computed result.
    inline float getOutput()
    {
        return output_;
    }

    /// Set the PID setpoint value.
    inline void setTarget(float target)
    {
        setpoint_ = target;
    }

    /// Get the PID proportional term.
    inline float getKp()
    {
        return GetKp();
    }

    /// Set the PID proportional term.
    inline void setKp(float kp)
    {
        SetTunings(kp, GetKi(), GetKd());
    }

    /// Get the PID integral term.
    inline float getKi()
    {
        return GetKi();
    }

    /// Set the PID integral term.
    inline void setKi(float ki)
    {
        SetTunings(GetKp(), ki, GetKd());
    }

    /// Get the PID derivative term.
    inline float getKd()
    {
        return GetKd();
    }

    /// Set the PID derivative term.
    inline void setKd(float kd)
    {
        SetTunings(GetKp(), GetKi(), kd);
    }

  private:
    float input_, output_, setpoint_ = 0;
};

#endif // _PID_CONTROLLER_H_
