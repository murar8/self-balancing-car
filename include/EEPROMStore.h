/// EEPROM access helpers.

#ifndef _EEPROM_STORE_H_
#define _EEPROM_STORE_H_

#include <EEPROM.h>
#include <inttypes.h>
#include <stddef.h>

class EEPROMStore
{
  public:
    /// Load the initial values into EEPROM.
    void initialize();

    inline float getGyroZeroAngle()
    {
        float zero_angle;
        return EEPROM.get(Address::GyroZeroAngle, zero_angle);
    };

    inline void setGyroZeroAngle(float zero_angle)
    {
        EEPROM.put(Address::GyroZeroAngle, zero_angle);
    };

    inline float getBalancePIDkP()
    {
        float kp;
        return EEPROM.get(Address::BalancePIDkP, kp);
    };

    inline void setBalancePIDkP(float kp)
    {
        EEPROM.put(Address::BalancePIDkP, kp);
    };

    inline float getBalancePIDkI()
    {
        float ki;
        return EEPROM.get(Address::BalancePIDkI, ki);
    };

    inline void setBalancePIDkI(float ki)
    {
        EEPROM.put(Address::BalancePIDkI, ki);
    };

    inline float getBalancePIDkD()
    {
        float kd;
        return EEPROM.get(Address::BalancePIDkD, kd);
    };

    inline void setBalancePIDkD(float kd)
    {
        EEPROM.put(Address::BalancePIDkD, kd);
    };

    inline float getVelocityPIDkP()
    {
        float kp;
        return EEPROM.get(Address::VelocityPIDkP, kp);
    };

    inline void setVelocityPIDkP(float kp)
    {
        EEPROM.put(Address::VelocityPIDkP, kp);
    };

    inline float getVelocityPIDkI()
    {
        float ki;
        return EEPROM.get(Address::VelocityPIDkI, ki);
    };

    inline void setVelocityPIDkI(float ki)
    {
        EEPROM.put(Address::VelocityPIDkI, ki);
    };

    inline float getVelocityPIDkD()
    {
        float kd;
        return EEPROM.get(Address::VelocityPIDkD, kd);
    };

    inline void setVelocityPIDkD(float kd)
    {
        EEPROM.put(Address::VelocityPIDkD, kd);
    };

  private:
    const size_t VERSION = 12;

    enum Address : int32_t
    {
        Version = sizeof(size_t),
        GyroZeroAngle = Version + sizeof(float),
        BalancePIDkP = GyroZeroAngle + sizeof(float),
        BalancePIDkI = BalancePIDkP + sizeof(float),
        BalancePIDkD = BalancePIDkI + sizeof(float),
        VelocityPIDkP = BalancePIDkD + sizeof(float),
        VelocityPIDkI = VelocityPIDkP + sizeof(float),
        VelocityPIDkD = VelocityPIDkI + sizeof(float),
    };
};

#endif // _EEPROM_UTIL_H_
