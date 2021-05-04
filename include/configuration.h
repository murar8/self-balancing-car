#ifndef _CONFIGURATION_H_
#define _CONFIGURATION_H_

/// Gyroscope module X axis accelerometer offset.
#define GYRO_OFFSET_ACCEL_X -1255
/// Gyroscope module Y axis accelerometer offset.
#define GYRO_OFFSET_ACCEL_Y -1113
/// Gyroscope module Z axis accelerometer offset.
#define GYRO_OFFSET_ACCEL_Z 1708

/// Gyroscope module X axis gyroscope offset.
#define GYRO_OFFSET_GYRO_X -17
/// Gyroscope module Y axis gyroscope offset.
#define GYRO_OFFSET_GYRO_Y 65
/// Gyroscope module Z axis gyroscope offset.
#define GYRO_OFFSET_GYRO_Z 6

/// [rad] Gyroscope module zero offset.
#define GYRO_ZERO_ANGLE -0.08
/// Gyroscope module i2c address.
#define GYRO_ADDRESS 0x68

/// Motor driver pin for running the left motor forward.
#define PIN_MOTOR_L_FW 6
/// Motor driver pin for running the left motor backwards.
#define PIN_MOTOR_L_BW 5

/// Motor driver pin for running the right motor forward.
#define PIN_MOTOR_R_FW 10
/// Motor driver pin for running the right motor backwards.
#define PIN_MOTOR_R_BW 9

/// Sample period of the balancing PID loop.
#define BALANCE_PID_SAMPLE_PERIOD 10
/// Default proportional parameter of the balancing PID loop.
#define BALANCE_PID_KP 650.0
/// Default integral parameter of the balancing PID loop.
#define BALANCE_PID_KI 3200.0
/// Default derivative parameter of the balancing PID loop.
#define BALANCE_PID_KD 45.0

/// Sample period of the velocity PID loop.
#define VELOCITY_PID_SAMPLE_PERIOD 100
/// Default proportional parameter of the velocity PID loop.
#define VELOCITY_PID_KP 0.0
/// Default integral parameter of the velocity PID loop.
#define VELOCITY_PID_KI 0.0
/// Default derivative parameter of the velocity PID loop.
#define VELOCITY_PID_KD 0.0

/// [°] Maximum inclination where the loop will try to stablize.
#define MAX_LEAN_ANGLE 40

/// [°] Inclination range where the robot will switch on and try to stabilize.
#define STARTUP_ANGLE 10

/// [ms] Lapse of time that the robot will need to stay upright within STATRTUP_ANGLE before turning on.
#define STARTUP_TIME 800

/// Maximum angle that can be requested where the vehicle is able to stay balanced.
#define MAX_WORKING_ANGLE 5

/// [°/s] Maximum rate of change of the angle before the robot will switch on and try to stabilize.
// #define STARTUP_ANGLE_DELTA 10

#endif
