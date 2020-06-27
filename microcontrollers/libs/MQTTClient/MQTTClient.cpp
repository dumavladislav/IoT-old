
#pragma once
#include "MQTTClient.h"
#include <JsonMessageBuilder.h>


MQTTClient::MQTTClient(char *devId)
{
    deviceId = devId;


}

void MQTTClient::init(char *mqttServer, int mqttPort, Client* networkClient) {
    client.setClient(*networkClient);
    client.setServer(strdup(mqttServer), mqttPort);
}

void MQTTClient::connect(char *mqttUsr, char *mqttPasswd)
{
    mqttConnect(mqttUsr, mqttPasswd);
}

boolean MQTTClient::isConnected() {
    return client.connected();
}

void MQTTClient::authorizationRequest() {
    //JsonObject data;
    sendJsonMessage(AUTHORIZATION_REQUESTS_TPC, data);
    // subscribe(AUTHORIZATION_REQUESTS_STATUS_TPC);   
} 



void MQTTClient::mqttConnect(char *mqttUsr, char *mqttPasswd)
{
    // Serial.print("Start MQTT Connect...");
    // Loop until we're reconnected
    while (!client.connected())
    {
        // Serial.println("Not Connected...");
        unsigned long now = millis();
        // convert now to string form
        // char *dt = ctime(&now);

        // Serial.println("==========================================");
        // Serial.println(deviceId);
        // Serial.println(String(random(0xffff), HEX).c_str());
        //Serial.println(dt);
        // Serial.println("==========================================");
        //snprintf(deviceSessionId, 50, "%s_%s", deviceId, String(random(0xffff), HEX).c_str());
        deviceSessionId = (char *)String(String(deviceId) + String(random(0xffff), HEX)).c_str();// + String(millis())).c_str();
        //deviceSessionId = (char*) ( String("sdfsdf") + String("_444") ).c_str(); 
        // Serial.println(String(String(deviceId) + String(random(0xffff), HEX)).c_str());
        // Serial.println(deviceSessionId);
        // Serial.println(mqttUsr);
        // Serial.println(mqttPasswd);
        // Serial.print("Attempting MQTT connection...");
        // Attempt to connect
        if (!client.connect(String(String(deviceId) + String(random(0xffff), HEX)).c_str(), mqttUsr, mqttPasswd))
        {
            // Serial.print("failed, rc=");
            // Serial.print(client.state());
            // Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
};



void MQTTClient::keepAlive(char *mqttUsr, char *mqttPasswd)
{   
    if (!client.connected())
    {
        mqttConnect(mqttUsr, mqttPasswd);
    }
    client.loop();
};

void MQTTClient::setCallback(MQTT_CALLBACK_SIGNATURE)
{
    client.setCallback(callback);
}

boolean MQTTClient::sendMessage(char *topicName, String message)
{
    Serial.print("Message to topic: ");
    Serial.println(topicName);
    Serial.println(message);
    if(!client.publish(topicName, message.c_str())) 
    {
        Serial.println("Message NOT published");
        return false;
    }
    return true;
}

void MQTTClient::sendJsonMessage(char *topicName, JsonObject json) {
    JsonMessageBuilder jsonMessageBuilder(authorizationBlock);
    Serial.println(111);
    jsonMessageBuilder.addElement("obj", "value1");
    Serial.println(222);
    // JsonObject nestObj;
    // nestObj["qqq"] = 111;
    // nestObj["ppp"] = "qwerty";
    // jsonMessageBuilder.addObject("nestedObj", nestObj);
    // Serial.println(jsonMessageBuilder.toString());
    sendMessage(AUTHORIZATION_REQUESTS_TPC, jsonMessageBuilder.toString());
}


void MQTTClient::subscribe(char* topicName) {
    client.subscribe(topicName);
}


