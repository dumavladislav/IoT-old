#pragma once

#include <Arduino.h>
#include <MQTTClient.h>
#include "Constants/Credentials.h"
#include <DeviceSettings.h>
#include "AuthorizationBlock.h"
#include <iostream>
#include <unordered_map>
#include <RCSwitch.h>

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
    void setDefaultPresets();
    void applyPreset(int presetNumber);
    
    // -------------------------- Operation ------------------------------
    void updateState(int newState);

    int getState();
    void setState(int newState);

    void resetTimer();


private:

    MQTTClient *mqttClient;

    boolean MSPreviousState = 0;
    boolean MSState = 0;

    // void sendMSState(boolean currState);
    // void sendOperationMode(int mode);

    operationModes decodeOperationMode(int intVar);
    void sendMSState();
    // void sendOperationMode();

    float startTimeOfOperation;

    void msDrivenOperation(int newState);
    void offOperation();
    void onOperation();
    String generateJson(std::unordered_map<String, String> hashmap);
    void settingsRequest();

    AuthorizationBlock authorizationBlock;
    DeviceSettings deviceSettings;
    std::vector<DeviceSettings> devicePresets;    
};