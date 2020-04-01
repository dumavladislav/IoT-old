#pragma once

#include <Arduino.h>
#include "MQTT/MQTTClient.h"
#include "Constants/Credentials.h"
#include "./Settings/DeviceSettings.h"
#include "AuthorizationBlock.h"
#include <iostream>
#include <unordered_map>

class MQTTLightControl /*: public IWifiDevice, public IMQTTDevice*/
{

public:
    MQTTLightControl(/*char *devId,*/ char *mqttServer, int mqttPort, uint32_t chipId);
    DeviceSettings getSettings();
    void setCallback(MQTT_CALLBACK_SIGNATURE);
    void connect();
    void keepAlive(char *mqttUsr, char *mqttPasswd);
    
    // ------------------------ Authorization ----------------------------
    void authorizationRequest();
    void authorizationResponse(String message);

    // -------------------------- Settings -------------------------------
    void applyNewSettings(String message);  // when MQTT message comes
    void setDeviceSettings(DeviceSettings newSettings); // setter
    
    // -------------------------- Operation ------------------------------
    void updateState(int newState);

    int getState();

private:

    MQTTClient *mqttClient;

    boolean MSPreviousState = 0;
    boolean MSState = 0;

    void sendMSState(boolean currState);
    void sendOperationMode(int mode);

    operationModes decodeOperationMode(int intVar);
    void sendMSState();
    void sendOperationMode();

    void msDrivenOperation(int newState);
    void offOperation();
    void onOperation();
    String generateJson(std::unordered_map<String, String> hashmap);
    void settingsRequest();

    AuthorizationBlock authorizationBlock;
    DeviceSettings deviceSettings;
};