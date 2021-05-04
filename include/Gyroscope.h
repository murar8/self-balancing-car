/// Single axis gyroscope driver for the MPU6050 accelerometer.

#ifndef _GYROSCOPE_H_
#define _GYROSCOPE_H_

// clang-format off
#include <inttypes.h>
#include <math.h>
#include <helper_3dmath.h>
#define MPU6050_INCLUDE_DMP_MOTIONAPPS20
#include <MPU6050.h>
// clang-format on

class Gyroscope
{
  public:
    struct Offset
    {
        int32_t x, y, z;
    };

    Gyroscope(uint8_t address, float zero_angle, Offset offset_accel, Offset offset_gyro);

    void begin();
    void tick();

    /// Read the current inclination in the range PI to -PI.
    float getAngle();

    /// Get the gyroscope zero offset.
    inline float getZeroAngle()
    {
        return zero_angle_;
    }

    /// Set the gyroscope zero offset.
    inline void setZeroAngle(float zero_angle)
    {
        zero_angle_ = zero_angle;
    }

  private:
    /// [ms] Lapse of time to wait for the output to stabilize,
    /// might need a longer timeout depending on initial orientation.
    static const uint32_t OUTPUT_STABILIZATION_DELAY = 1000;

    uint8_t address_;
    float zero_angle_;
    Offset offset_accel_, offset_gyro_;

    MPU6050 mpu_;
    uint8_t fifo_buffer_[64];
};

#endif
