

#pragma once
#include "../Network/INetworkConnect.h"
#include <Arduino.h>
#include <PubSubClient.h>
#include "../Constants/Constants.h"


class MQTTClient
{

public:
    MQTTClient(char *devId, char *mqttServer, int mqttPort, Client* networkClient);
    void    connect(char *mqttUsr, char *mqttPasswd);
    void    autorizationRequest();
    void    keepAlive(char *mqttUsr, char *mqttPasswd);
    void    setCallback(MQTT_CALLBACK_SIGNATURE);
    void    sendMessage(char *topicName, String message);
    void    subscribe(char *topicName);

private:
    char *deviceId;
    char *deviceSessionId;

    PubSubClient client;

    void mqttConnect(char *mqttUsr, char *mqttPasswd);
};