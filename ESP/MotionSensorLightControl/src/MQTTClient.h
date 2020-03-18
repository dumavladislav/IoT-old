#pragma once

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "Constants.h"

class MQTTClient
{

public:
    MQTTClient(char *devId, char *mqttServer, int mqttPort);
    void connect(char *ssid, char *password, char *mqttUsr, char *mqttPasswd);
    void keepAlive(char *mqttUsr, char *mqttPasswd);
    void setCallback(MQTT_CALLBACK_SIGNATURE);
    void sendMessage(char *topicName, char message[]);

private:
    char *deviceId;
    char *deviceSessionId;

    WiFiClient espClient;
    PubSubClient client;

    void wifiConnect(char *ssid, char *password);
    void mqttConnect(char *mqttUsr, char *mqttPasswd);
};