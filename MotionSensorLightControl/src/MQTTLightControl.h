#pragma once

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "IWifiDevice.h"
#include "IMQTTDevice.h"
#include "Constants.h"


enum operationModes { MSDRIVEN, ON, OFF };

class MQTTLightControl: public IWifiDevice, public IMQTTDevice {

public:
    MQTTLightControl(char* devId, char* mqttServer, int mqttPort/*, MQTT_CALLBACK_SIGNATURE*/);
    void setCallback(MQTT_CALLBACK_SIGNATURE);
    void wifiConnect(char* ssid, char* password);
    void mqttConnect(char* mqttUsr, char* mqttPasswd);
    void sendMessage(char* topicName, char message[]);

    void keepAlive(char* mqttUsr, char* mqttPasswd);
    void updateState(int newState);
    
    operationModes getOperationMode();
    void setOperationMode(int mode);

    int getState();
    
private:

    char* deviceId;
    char* deviceSessionId;

    PubSubClient client;

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