#pragma once

#include <Arduino.h>
#include <PubSubClient.h>
#include "IWifiDevice.h"
#include "IMQTTDevice.h"
#include "MQTTClient.h"
#include "Constants.h"
#include "Credentials.h"

enum operationModes
{
    MSDRIVEN,
    ON,
    OFF
};

class MQTTLightControl /*: public IWifiDevice, public IMQTTDevice*/
{

public:
    MQTTLightControl(char *devId, char *mqttServer, int mqttPort /*, MQTT_CALLBACK_SIGNATURE*/);
    void setCallback(MQTT_CALLBACK_SIGNATURE);
    void connect();
    void keepAlive(char *mqttUsr, char *mqttPasswd);

    void updateState(int newState);

    operationModes getOperationMode();
    void setOperationMode(int mode);

    int getState();

private:
    MQTTClient *mqttClient;

    operationModes operationMode = operationModes::MSDRIVEN;
    boolean MSPreviousState = 0;
    boolean MSState = 0;
    float startTimeOnOperationMode = 0;

    void sendMSState(boolean currState);
    void sendOperationMode(int mode);

    operationModes decodeOperationMode(int intVar);
    void sendMSState();
    void sendOperationMode();

    void msDrivenOperation(int newState);
    void offOperation();
    void onOperation();
};