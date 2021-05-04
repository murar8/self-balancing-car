#include "EEPROMStore.h"
#include "configuration.h"

void EEPROMStore::initialize()
{
    size_t version;
    EEPROM.get(Address::Version, version);

    if (version == VERSION)
    {
        return;
    }

    EEPROM.put(Address::Version, VERSION);
    EEPROM.put(Address::GyroZeroAngle, GYRO_ZERO_ANGLE);
    EEPROM.put(Address::BalancePIDkP, BALANCE_PID_KP);
    EEPROM.put(Address::BalancePIDkI, BALANCE_PID_KI);
    EEPROM.put(Address::BalancePIDkD, BALANCE_PID_KD);
    EEPROM.put(Address::VelocityPIDkP, VELOCITY_PID_KP);
    EEPROM.put(Address::VelocityPIDkI, VELOCITY_PID_KI);
    EEPROM.put(Address::VelocityPIDkD, VELOCITY_PID_KD);
}
