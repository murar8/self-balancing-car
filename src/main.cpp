#include "CommunicationManager.h"
#include "EEPROMStore.h"
#include "Encoder.h"
#include "Gyroscope.h"
#include "Motor.h"
#include "PIDController.h"
#include "configuration.h"
#include "convert.h"
#include <Arduino.h>

static const constexpr float MAX_LEAN_ANGLE_RAD = (MAX_LEAN_ANGLE * M_PI) / 180.0;
static const constexpr float STARTUP_ANGLE_RAD = (STARTUP_ANGLE * M_PI) / 180.0;
static const constexpr float MAX_WORKING_ANGLE_RAD = (MAX_WORKING_ANGLE * M_PI) / 180.0;

EEPROMStore eeprom_store;

Gyroscope gyroscope(GYRO_ADDRESS,
                    GYRO_ZERO_ANGLE,
                    Gyroscope::Offset{GYRO_OFFSET_ACCEL_X, GYRO_OFFSET_ACCEL_Y, GYRO_OFFSET_ACCEL_Z},
                    Gyroscope::Offset{GYRO_OFFSET_GYRO_X, GYRO_OFFSET_GYRO_Y, GYRO_OFFSET_GYRO_Z});

Motor motor_left(PIN_MOTOR_L_FW, PIN_MOTOR_L_BW);
Motor motor_right(PIN_MOTOR_R_FW, PIN_MOTOR_R_BW);

Encoder encoder_left(ENCODER_SAMPLE_PERIOD, PIN_ENCODER_L_A, PIN_ENCODER_L_B, PIN_ENCODER_L_FW_LEVEL);
Encoder encoder_right(ENCODER_SAMPLE_PERIOD, PIN_ENCODER_R_A, PIN_ENCODER_R_B, PIN_ENCODER_R_FW_LEVEL);

CommunicationManager comm_manager;

PIDController balance_loop(BALANCE_PID_SAMPLE_PERIOD, static_cast<double>(INT8_MIN), static_cast<double>(INT8_MAX));
PIDController velocity_loop(VELOCITY_PID_SAMPLE_PERIOD, -MAX_WORKING_ANGLE_RAD, MAX_WORKING_ANGLE_RAD, false);

Handler handlers[] = {
    Handler{
        .name = "s",
        .get = nullptr,
        .set =
            [](char *buffer) {
                double speed = stringToDouble(buffer);
                if (isnan(speed)) return true;
                velocity_loop.setTarget(speed);
                return false;
            },
    },

    Handler{
        .name = "d",
        .get = nullptr,
        .set =
            [](char *buffer) {
                double dir = stringToDouble(buffer);
                if (isnan(dir)) return true;
                velocity_loop.setTarget(dir);
                return false;
            },
    },

    Handler{
        .name = "gyroscope.zero-angle",
        .get =
            [](char *buffer) {
                dtostrf(gyroscope.getZeroAngle(), 0, 2, buffer);
                return false;
            },
        .set =
            [](char *buffer) {
                double zero_angle = stringToDouble(buffer);
                if (isnan(zero_angle)) return true;
                eeprom_store.setGyroZeroAngle(zero_angle);
                gyroscope.setZeroAngle(zero_angle);
                return false;
            },
    },

    Handler{
        .name = "balance-pid.kp",
        .get =
            [](char *buffer) {
                dtostrf(balance_loop.getKp(), 0, 2, buffer);
                return false;
            },
        .set =
            [](char *buffer) {
                double kp = stringToDouble(buffer);
                if (isnan(kp)) return true;
                eeprom_store.setBalancePIDkP(kp);
                balance_loop.setKp(kp);
                return false;
            },
    },

    Handler{
        .name = "balance-pid.ki",
        .get =
            [](char *buffer) {
                dtostrf(balance_loop.getKi(), 0, 2, buffer);
                return false;
            },
        .set =
            [](char *buffer) {
                double ki = stringToDouble(buffer);
                if (isnan(ki)) return true;
                eeprom_store.setBalancePIDkI(ki);
                balance_loop.setKi(ki);
                return false;
            },
    },

    Handler{
        .name = "balance-pid.kd",
        .get =
            [](char *buffer) {
                dtostrf(balance_loop.getKd(), 0, 2, buffer);
                return false;
            },
        .set =
            [](char *buffer) {
                double kd = stringToDouble(buffer);
                if (isnan(kd)) return true;
                eeprom_store.setBalancePIDkD(kd);
                balance_loop.setKd(kd);
                return false;
            },
    },

    Handler{
        .name = "velocity-pid.kp",
        .get =
            [](char *buffer) {
                dtostrf(velocity_loop.getKp(), 0, 8, buffer);
                return false;
            },
        .set =
            [](char *buffer) {
                double kp = stringToDouble(buffer);
                if (isnan(kp)) return true;
                eeprom_store.setVelocityPIDkP(kp);
                velocity_loop.setKp(kp);
                return false;
            },
    },

    Handler{
        .name = "velocity-pid.ki",
        .get =
            [](char *buffer) {
                dtostrf(velocity_loop.getKi(), 0, 8, buffer);
                return false;
            },
        .set =
            [](char *buffer) {
                double ki = stringToDouble(buffer);
                if (isnan(ki)) return true;
                eeprom_store.setVelocityPIDkI(ki);
                velocity_loop.setKi(ki);
                return false;
            },
    },

    Handler{
        .name = "velocity-pid.kd",
        .get =
            [](char *buffer) {
                dtostrf(velocity_loop.getKd(), 0, 8, buffer);
                return false;
            },
        .set =
            [](char *buffer) {
                double kd = stringToDouble(buffer);
                if (isnan(kd)) return true;
                eeprom_store.setVelocityPIDkD(kd);
                velocity_loop.setKd(kd);
                return false;
            },
    },
};

void loadEEPROM()
{
    eeprom_store.initialize();

    float balance_pid_kp = eeprom_store.getBalancePIDkP();
    float balance_pid_ki = eeprom_store.getBalancePIDkI();
    float balance_pid_kd = eeprom_store.getBalancePIDkD();
    balance_loop.setKp(balance_pid_kp);
    balance_loop.setKi(balance_pid_ki);
    balance_loop.setKd(balance_pid_kd);

    float velocity_pid_kp = eeprom_store.getVelocityPIDkP();
    float velocity_pid_ki = eeprom_store.getVelocityPIDkI();
    float velocity_pid_kd = eeprom_store.getVelocityPIDkD();
    velocity_loop.setKp(velocity_pid_kp);
    velocity_loop.setKi(velocity_pid_ki);
    velocity_loop.setKd(velocity_pid_kd);

    float gyro_zero_angle = eeprom_store.getGyroZeroAngle();
    gyroscope.setZeroAngle(gyro_zero_angle);
}

void setup()
{
    loadEEPROM();

    Serial.begin(9600);
    comm_manager.begin(&Serial, handlers);

    gyroscope.begin();

    motor_left.begin();
    motor_right.begin();

    encoder_left.begin();
    encoder_right.begin();

    velocity_loop.enable();
}

void setStartedStopped(float angle)
{
    static bool starting_, started_ = false;
    static uint32_t start_timestamp_;

    float abs_angle = abs(angle);

    if (abs_angle > MAX_LEAN_ANGLE_RAD && started_)
    {
        balance_loop.disable();
        velocity_loop.disable();
        started_ = false;
    }
    else if (!started_)
    {
        if (abs_angle > STARTUP_ANGLE_RAD)
        {
            starting_ = false;
        }

        if (!starting_)
        {
            starting_ = true;
            start_timestamp_ = millis();
        }

        if (millis() - start_timestamp_ > STARTUP_TIME)
        {
            balance_loop.enable();
            velocity_loop.enable();
            starting_ = false;
            started_ = true;
        }
    }
}

void setDesiredAngle()
{
    static bool encoder_l_ready, encoder_r_ready = false;

    if (!encoder_l_ready)
    {
        encoder_l_ready = encoder_left.tick();
    }

    if (!encoder_r_ready)
    {
        encoder_r_ready = encoder_right.tick();
    }

    if (encoder_l_ready && encoder_r_ready)
    {
        float speed_l = encoder_left.getFrequency();
        float speed_r = encoder_right.getFrequency();
        float speed_avg = (speed_l + speed_r) / 2;

        if (velocity_loop.compute(speed_avg))
        {
            balance_loop.setTarget(velocity_loop.getOutput());
            encoder_l_ready = false;
            encoder_r_ready = false;
        }
    }
}

void setDesiredDuty(float angle)
{
    if (balance_loop.compute(angle))
    {
        motor_left.setDuty(balance_loop.getOutput());
        motor_right.setDuty(balance_loop.getOutput());
    }
}

void loop()
{
    comm_manager.tick();
    gyroscope.tick();

    float angle = gyroscope.getAngle();

    setDesiredAngle();
    setDesiredDuty(angle);
    setStartedStopped(angle);
}
