

#pragma once
#include <INetworkConnect.h>
#include <Arduino.h>
#include <PubSubClient.h>
#include <GenericConstants.h>
#include <AuthorizationBlock.h>
#include <MessageBuilder.h>
//#include "../Constants/Constants.h"


class MQTTClient
{

public:
    MQTTClient(char *devId, char *mqttServer, int mqttPort, Client* networkClient);
    void    connect(char *mqttUsr, char *mqttPasswd);
    void    authorizationRequest(AuthorizationBlock authorizationBlock);
    void    keepAlive(char *mqttUsr, char *mqttPasswd);
    void    setCallback(MQTT_CALLBACK_SIGNATURE);
    void    sendMessage(char *topicName, String message);
    void    subscribe(char *topicName);

private:
    char *deviceId;
    char *deviceSessionId;

    AuthorizationBlock authorizationBlock;

    PubSubClient client;

    void mqttConnect(char *mqttUsr, char *mqttPasswd);
};