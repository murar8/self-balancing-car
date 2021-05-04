/// Quadrature encoder driver implementation with velocity estimation.

#ifndef _ENCODER_H_
#define _ENCODER_H_

#define CIRCULAR_BUFFER_INT_SAFE
#include <CircularBuffer.h>
#include <inttypes.h>

#define PIN_ENCODER_L_A 2
#define PIN_ENCODER_L_B 4
#define PIN_ENCODER_L_FW_LEVEL LOW

#define PIN_ENCODER_R_A 3
#define PIN_ENCODER_R_B 7
#define PIN_ENCODER_R_FW_LEVEL HIGH

class Encoder
{
  public:
    Encoder(uint8_t pin_phase_a, uint8_t pin_phase_b, uint8_t fw_level);

    void begin();
    void tick();

    float getFrequency();

    inline void onPulse();

  private:
    /// Length of the pulse buffer.
    static const size_t PULSE_BUFFER_LEN = 8;

    /// Number of encoder pulses for each motor revolution.
    static const size_t PULSES_PER_REVOLUTION = 8;

    /// Minimum time interval between encoder pulses to protect against spurious reads [us].
    static const constexpr float MINIMUM_PULSE_DURATION = 600;

    enum class Direction
    {
        NONE,
        CW,
        CCW,
    };

    struct Pulse
    {
        Direction direction;
        uint32_t timestamp;
    };

    uint8_t pin_phase_a_, pin_phase_b_, fw_level_;
    uint8_t port_phase_b_, bit_phase_b_;

    CircularBuffer<Pulse, PULSE_BUFFER_LEN> pulse_buffer_;

    uint32_t last_period_;
    Pulse last_pulse_ = Pulse{.direction = Direction::NONE};
};

#endif
