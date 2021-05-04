/// MX1616 DC motor driver implementation.

#ifndef _MOTOR_H_
#define _MOTOR_H_

#include <Arduino.h>

class Motor
{
  public:
    Motor(uint8_t pin_fw, uint8_t pin_bw);

    /// Initialize the motor hardware.
    void begin();

    /// Set the motor duty cycle and direction.
    void setDuty(int8_t duty);

  private:
    uint8_t pin_fw_, pin_bw_;

    static bool synchronized;

    /// Set all PWM outputs to the same 1kHz frequency.
    static void syncTimers();
};

#endif
