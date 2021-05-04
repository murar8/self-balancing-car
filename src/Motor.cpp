#include "Motor.h"

bool Motor::synchronized = false;

void Motor::syncTimers()
{
    GTCCR |= (1 << TSM) | (1 << PSRASY) | (1 << PSRSYNC); // Halt all timers.

    TCCR1A |= (1 << WGM10); // Fast PWM, TOP = 0x00FF.
    TCCR1A &= ~(1 << WGM11);
    TCCR1B &= ~(1 << WGM13);
    TCCR1B |= (1 << WGM12);

    TCCR2A |= (1 << WGM21) | (1 << WGM20); // Fast PWM, TOP = 0xFF.
    TCCR2B &= ~(1 << WGM22);

    TCNT0 = 0; // Reset counters.
    TCNT1 = 0;
    TCNT2 = 0;

    GTCCR = 0; // Start all timers
}

Motor::Motor(uint8_t pin_fw, uint8_t pin_bw) : pin_fw_(pin_fw), pin_bw_(pin_bw){};

void Motor::begin()
{
    if (!synchronized)
    {
        syncTimers();
        synchronized = true;
    }

    pinMode(pin_fw_, OUTPUT);
    pinMode(pin_bw_, OUTPUT);
}

void Motor::setDuty(int8_t duty)
{
    uint8_t abs_duty = duty >= 0 ? duty * 2 + 1 : -(duty + 1) * 2;

    if (duty >= 0)
    {
        analogWrite(pin_fw_, abs_duty);
        digitalWrite(pin_bw_, LOW);
    }
    else
    {
        analogWrite(pin_bw_, abs_duty);
        digitalWrite(pin_fw_, LOW);
    }
}
