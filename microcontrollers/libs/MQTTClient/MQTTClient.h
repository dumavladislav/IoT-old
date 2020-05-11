

#pragma once
#include <INetworkConnect.h>
#include <PubSubClient.h>
#include <GenericConstants.h>
#include <AuthorizationBlock.h>
#include <ArduinoJson.h>

//#define MQTT_SOCKET_TIMEOUT 60

class MQTTClient
{

public:
    MQTTClient(char *devId);
    void init(char *mqttServer, int mqttPort, Client* networkClient);
    void    connect(char *mqttUsr, char *mqttPasswd);
    boolean isConnected();
    void    authorizationRequest();
    void    keepAlive(char *mqttUsr, char *mqttPasswd);
    void    setCallback(MQTT_CALLBACK_SIGNATURE);
    boolean    sendMessage(char *topicName, String message);
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