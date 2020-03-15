#include "MQTTClient.h"

MQTTClient::MQTTClient(char *devId, char *mqttServer, int mqttPort /*, MQTT_CALLBACK_SIGNATURE*/)
{
    deviceId = devId;

    client = PubSubClient(espClient);
    client.setServer(mqttServer, mqttPort);
}

void MQTTClient::connect(char *ssid, char *password, char *mqttUsr, char *mqttPasswd)
{
    wifiConnect(ssid, password);
    mqttConnect(mqttUsr, mqttPasswd);
}

void MQTTClient::wifiConnect(char *ssid, char *password)
{
    delay(10);
    // We start by connecting to a WiFi network
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    randomSeed(micros());

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
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
        deviceSessionId = (char *)((String)deviceId + String(random(0xffff), HEX)).c_str();
        Serial.println(deviceSessionId);
        Serial.print("Attempting MQTT connection...");
        // Attempt to connect
        if (client.connect(deviceSessionId, mqttUsr, mqttPasswd))
        {
            // Once connected, publish an announcement...
            client.publish(GREETING_TPC, ((String)deviceSessionId + " connected").c_str());
            // ... and resubscribe
            client.subscribe(DEVICE_OPERATION_CONTROL_TPC);
        }
        else
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

void MQTTClient::sendMessage(char *topicName, char message[])
{
    char msg[50];
    // current date/time based on current system
    time_t now = time(0);
    // convert now to string form
    char *dt = ctime(&now);
    snprintf(msg, 50, "%s%s: %s", dt, deviceSessionId, message);
    client.publish(topicName, msg);
}
