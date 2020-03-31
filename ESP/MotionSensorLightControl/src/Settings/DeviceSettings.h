#include <Arduino.h>
#include "./Constants/Constants.h"

enum operationModes
{
    MSDRIVEN,
    ON,
    OFF
};

struct DeviceSettings
{
    String deviceId = "HALL_SENSOR_TABLE_1";
    operationModes operationMode = operationModes::MSDRIVEN;
    int relayMode = 0;
    int maxOnOperationModeDuration = 60000;
    
};
