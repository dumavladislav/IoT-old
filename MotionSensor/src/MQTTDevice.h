#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "IWifiDevice.h"
#include "IMQTTDevice.h"

class MQTTDevice: IWifiDevice, IMQTTDevice {

public:
    MQTTDevice(String deviceId);

    void connect();
    void reconnect(String clientId);
    void sendMessage(char* topicName, char message[]);

private:
    WiFiClient espClient;
    PubSubClient client(WiFiClient espClient);

    

}