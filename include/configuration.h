#ifndef _CONFIGURATION_H_
#define _CONFIGURATION_H_

// Gyroscope

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

// Motors

/// Motor driver pin for running the left motor forward.
#define PIN_MOTOR_L_FW 6
/// Motor driver pin for running the left motor backwards.
#define PIN_MOTOR_L_BW 5

/// Motor driver pin for running the right motor forward.
#define PIN_MOTOR_R_FW 10
/// Motor driver pin for running the right motor backwards.
#define PIN_MOTOR_R_BW 9

// PID loops

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

// Encoders

/// Number of encoder pulses for each motor revolution.
#define ENCODER_PULSES_PER_REVOLUTION 8

/// [ms] Sample period of the encoder.
/// Please note that VELOCITY_PID_SAMPLE_PERIOD should be a multiple of ENCODER_SAMPLE_PERIOD.
#define ENCODER_SAMPLE_PERIOD VELOCITY_PID_SAMPLE_PERIOD

/// Arduino pin connected to the left encoder's A phase.
#define PIN_ENCODER_L_A 2
/// Arduino pin connected to the left encoder's B phase.
#define PIN_ENCODER_L_B 4
/// Logic level of the phase B pin when receiving a pulse on phase A while going forward.
#define PIN_ENCODER_L_FW_LEVEL LOW

/// Arduino pin connected to the right encoder's A phase.
#define PIN_ENCODER_R_A 3
/// Arduino pin connected to the right encoder's B phase.
#define PIN_ENCODER_R_B 7
/// Logic level of the phase B pin when receiving a pulse on phase A while going forward.
#define PIN_ENCODER_R_FW_LEVEL HIGH

// Startup

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
