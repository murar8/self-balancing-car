#include "Encoder.h"

#include <PinChangeInterrupt.h>
#include <util/atomic.h>

#define ISR_HANDLER(n) [] { instances[n]->_onPulse(); }

typedef void (*Handler)(void);

static Encoder *instances[2];
static Handler handlers[2] = {ISR_HANDLER(0), ISR_HANDLER(1)};

Encoder::Encoder(float sample_time, uint8_t pin_phase_a, uint8_t pin_phase_b, uint8_t fw_level)
    : sample_time_(sample_time), pin_phase_a_(pin_phase_a), pin_phase_b_(pin_phase_b), fw_level_(fw_level)
{
    port_phase_b_ = digitalPinToPort(pin_phase_b_);
    bit_phase_b_ = digitalPinToBitMask(pin_phase_b_);
    last_sample_ = millis();
};

void Encoder::begin()
{
    pinMode(pin_phase_a_, INPUT_PULLUP);
    pinMode(pin_phase_b_, INPUT_PULLUP);

    uint8_t int_vect = digitalPinToInterrupt(pin_phase_a_);
    instances[int_vect] = this;
    attachInterrupt(int_vect, handlers[int_vect], RISING);
}

inline void Encoder::_onPulse()
{
    uint8_t level = (*portInputRegister(port_phase_b_) & bit_phase_b_) != 0;
    Direction next_direction = level ^ fw_level_ ? Direction::CW : Direction::CCW;

    if (next_direction == direction_)
    {
        pulse_count_++;
    }
    else
    {
        pulse_count_ = 1;
    }

    direction_ = next_direction;
}

bool Encoder::tick()
{
    uint32_t now = millis();

    if (now - last_sample_ < sample_time_)
    {
        return false;
    }

    frequency_ = (static_cast<float>(pulse_count_) * (1000.0 / ENCODER_PULSES_PER_REVOLUTION)) / sample_time_;
    pulse_count_ = 0;
    last_sample_ = now;

    if (direction_ == Direction::CCW)
    {
        frequency_ *= -1;
    }

    return true;
}

float Encoder::getFrequency()
{
    return frequency_;
}
