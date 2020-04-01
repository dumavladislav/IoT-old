#pragma once
#include <Arduino.h>
#include "./Constants/Constants.h"
#include "./MotionSensorSettings.h"

enum operationModes
{
    MSDRIVEN = 0,
    ON = 1,
    OFF = 2
};

struct DeviceSettings
{
    String deviceId = "HALL_SENSOR_TABLE_1";
    operationModes operationMode = operationModes::MSDRIVEN;
    MotionSensorSettings motionSensorSettings;
};




