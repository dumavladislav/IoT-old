#pragma once

#include <Arduino.h>
#include <PubSubClient.h>
#include "MQTT/MQTTClient.h"
#include "Constants/Constants.h"
#include "Constants/Credentials.h"
#include "AuthorizationBlock.h"
#include <iostream>
#include <unordered_map>
//#include "INetworkConnect.h"

enum operationModes
{
    MSDRIVEN,
    ON,
    OFF
};

class MQTTLightControl /*: public IWifiDevice, public IMQTTDevice*/
{

public:
    MQTTLightControl(char *devId, char *mqttServer, int mqttPort, uint32_t chipId);
    void setCallback(MQTT_CALLBACK_SIGNATURE);
    void connect();
    void keepAlive(char *mqttUsr, char *mqttPasswd);
    
    // ------------------------ Authorization ----------------------------
    void authorizationRequest();
    void authorizationResponse(String message);
    
    // -------------------------- Operation ------------------------------
    void updateState(int newState);

    operationModes getOperationMode();
    void setOperationMode(int mode);

    int getState();

private:

    MQTTClient *mqttClient;

    operationModes operationMode = operationModes::MSDRIVEN;
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
};