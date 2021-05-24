/// Quadrature encoder driver implementation.

#ifndef _ENCODER_H_
#define _ENCODER_H_

#include "configuration.h"
#include <stddef.h>
#include <stdint.h>

class Encoder
{
  public:
    Encoder(float sample_time, uint8_t pin_phase_a, uint8_t pin_phase_b, uint8_t fw_level);

    /// Set up the hardware peripherals and enable the interrupt handler.
    void begin();

    /// Perform the calculations necessary to update the frequency value.
    bool tick();

    /// Get the last measured frequency.
    float getFrequency();

    /// Private method.
    inline void _onPulse();

  private:
    /// Length of the pulse buffer.
    static const size_t PULSE_BUFFER_LEN = 8;

    enum class Direction
    {
        NONE,
        CW,
        CCW,
    };

    float sample_time_;
    uint32_t last_sample_;

    uint8_t pin_phase_a_, pin_phase_b_, fw_level_;
    uint8_t port_phase_b_, bit_phase_b_;

    uint32_t pulse_count_;
    Direction direction_;

    float frequency_;
};

#endif
