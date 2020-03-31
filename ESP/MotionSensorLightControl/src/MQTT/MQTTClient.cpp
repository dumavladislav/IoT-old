
#include "MQTT/MQTTClient.h"

MQTTClient::MQTTClient(char *devId, char *mqttServer, int mqttPort, Client* networkClient)
{
    deviceId = devId;

    client = PubSubClient(*networkClient);
    client.setServer(mqttServer, mqttPort);
}

void MQTTClient::connect(char *mqttUsr, char *mqttPasswd)
{
    mqttConnect(mqttUsr, mqttPasswd);
}



void MQTTClient::mqttConnect(char *mqttUsr, char *mqttPasswd)
{
    Serial.print("Start MQTT Connect...");
    // Loop until we're reconnected
    while (!client.connected())
    {
        Serial.println("Not Connected...");
        time_t now = time(0);
        // convert now to string form
        char *dt = ctime(&now);

        Serial.println("==========================================");
        Serial.println(deviceId);
        Serial.println(String(random(0xffff), HEX).c_str());
        Serial.println(dt);
        Serial.println("==========================================");
        //snprintf(deviceSessionId, 50, "%s_%s", deviceId, String(random(0xffff), HEX).c_str());
        //deviceSessionId = (char *)((String)deviceId + String(random(0xffff), HEX)).c_str();
        deviceSessionId = (char*) ( String("sdfsdf") + String("_444") ).c_str(); 
        Serial.println(deviceSessionId);
        Serial.print("Attempting MQTT connection...");
        // Attempt to connect
        if (!client.connect("QQQ", mqttUsr, mqttPasswd))
        {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
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

void MQTTClient::sendMessage(char *topicName, String message)
{
    if(!client.publish(topicName, message.c_str())) Serial.println("Message NOT published");
}


void MQTTClient::subscribe(char* topicName) {
    client.subscribe(topicName);
}
