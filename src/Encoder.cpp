#include "Encoder.h"

#include <PinChangeInterrupt.h>
#include <util/atomic.h>

#define ISR_HANDLER(n) [] { instances[n]->onPulse(); }

typedef void (*Handler)(void);

static Encoder *instances[2];
static Handler handlers[2] = {ISR_HANDLER(0), ISR_HANDLER(1)};

Encoder::Encoder(uint8_t pin_phase_a, uint8_t pin_phase_b, uint8_t fw_level)
    : pin_phase_a_(pin_phase_a), pin_phase_b_(pin_phase_b), fw_level_(fw_level)
{
    port_phase_b_ = digitalPinToPort(pin_phase_b_);
    bit_phase_b_ = digitalPinToBitMask(pin_phase_b_);
};

void Encoder::begin()
{
    pinMode(pin_phase_a_, INPUT_PULLUP);
    pinMode(pin_phase_b_, INPUT_PULLUP);

    uint8_t int_vect = digitalPinToInterrupt(pin_phase_a_);
    instances[int_vect] = this;
    attachInterrupt(int_vect, handlers[int_vect], RISING);
}

inline void Encoder::onPulse()
{
    uint8_t level = (*portInputRegister(port_phase_b_) & bit_phase_b_) != 0;
    Direction direction = level ^ fw_level_ ? Direction::CW : Direction::CCW;
    uint32_t timestamp = micros();
    pulse_buffer_.push(Pulse{direction, timestamp});
}

void Encoder::tick()
{
    for (;;)
    {
        if (pulse_buffer_.isEmpty())
        {
            if (last_pulse_.direction != Direction::NONE)
            {
                uint32_t elapsed_time = micros() - last_pulse_.timestamp;

                if (elapsed_time > last_period_)
                {
                    last_period_ = elapsed_time;
                }
            }
            return;
        }

        Pulse next_pulse = pulse_buffer_.shift();
        uint32_t next_period = next_pulse.timestamp - last_pulse_.timestamp;
        Direction old_direction = last_pulse_.direction;

        last_pulse_ = next_pulse;

        if (old_direction != Direction::NONE && next_pulse.direction != old_direction)
        {
            continue;
        }

        if (next_period < MINIMUM_PULSE_DURATION)
        {
            continue;
        }

        last_period_ = next_period;
    }
}

float Encoder::getFrequency()
{
    if (last_pulse_.direction == Direction::NONE)
    {
        return 0;
    }

    float frequency = (1000000 / PULSES_PER_REVOLUTION) / static_cast<float>(last_period_);

    if (last_pulse_.direction == Direction::CCW)
    {
        frequency = -frequency;
    }

    return frequency;
}
