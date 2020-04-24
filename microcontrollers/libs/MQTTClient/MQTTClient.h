

#pragma once
#include <INetworkConnect.h>
#include <PubSubClient.h>
#include <GenericConstants.h>
#include <AuthorizationBlock.h>
#include <ArduinoJson.h>

class MQTTClient
{

public:
    MQTTClient(char *devId, char *mqttServer, int mqttPort, Client* networkClient);
    void    connect(char *mqttUsr, char *mqttPasswd);
    void    authorizationRequest();
    void    keepAlive(char *mqttUsr, char *mqttPasswd);
    void    setCallback(MQTT_CALLBACK_SIGNATURE);
    void    sendMessage(char *topicName, String message);
    void    sendJsonMessage(char *topicName, JsonObject json);
    void    subscribe(char *topicName);


    void addElement(String elementName, String elementValue);
    void addObject(String objectName, JsonObject jsonObject);
    void addArrayOfObjects(String arrayName, JsonObject jsonObject[]);
    String toString();

private:
    char *deviceId;
    char *deviceSessionId;

    AuthorizationBlock authorizationBlock;

    PubSubClient client;

    void mqttConnect(char *mqttUsr, char *mqttPasswd);


    DynamicJsonDocument* jsonDoc;
    JsonObject header;
    JsonObject data;
};