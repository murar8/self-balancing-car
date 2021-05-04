#include "gyroscope.h"
#include <MPU6050_6Axis_MotionApps_V6_12.h>
#include <assert.h>

Gyroscope::Gyroscope(uint8_t address, float zero_angle, Offset offset_accel, Offset offset_gyro)
    : address_(address), zero_angle_(zero_angle), offset_accel_(offset_accel), offset_gyro_(offset_gyro), mpu_(address)
{
    assert(zero_angle <= PI && zero_angle >= -PI);
};

void Gyroscope::begin()
{
    Wire.begin();
    Wire.setClock(400000);
    Wire.setWireTimeout(3000, true);

    mpu_.initialize();

    assert(mpu_.testConnection());
    assert(mpu_.dmpInitialize() == 0);

    mpu_.setXAccelOffset(offset_accel_.x);
    mpu_.setYAccelOffset(offset_accel_.y);
    mpu_.setZAccelOffset(offset_accel_.z);
    mpu_.setXGyroOffset(offset_gyro_.x);
    mpu_.setYGyroOffset(offset_gyro_.y);
    mpu_.setZGyroOffset(offset_gyro_.z);

    mpu_.setDMPEnabled(true);

    delay(OUTPUT_STABILIZATION_DELAY);
}

void Gyroscope::tick()
{
    if (mpu_.dmpPacketAvailable())
    {
        mpu_.dmpGetCurrentFIFOPacket(fifo_buffer_);
    }
}

float Gyroscope::getAngle()
{
    Quaternion quaternion;
    VectorFloat gravity;
    float ypr[3];

    mpu_.dmpGetQuaternion(&quaternion, fifo_buffer_);
    mpu_.dmpGetGravity(&gravity, &quaternion);
    mpu_.dmpGetYawPitchRoll(ypr, &quaternion, &gravity);

    float pitch = ypr[1] + zero_angle_;

    if (pitch < -PI)
    {
        return 2 * PI + pitch;
    }

    if (pitch > PI)
    {
        return -2 * PI + pitch;
    }

    return pitch;
}
