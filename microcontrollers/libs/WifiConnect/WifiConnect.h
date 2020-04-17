#include "INetworkConnect.h"
#include <ESP8266WiFi.h>

class WifiConnect: public INetworkConnect {

public:

    WifiConnect(char *ssid, char *password);
    WiFiClient* getNetworkClient();
    
    IPAddress getIpAddress();
    String getMacAddress();

    WiFiClient* connect();
    bool isConnected();
    void disconnect();


private:
    char *ssid;
    char *password;
    WiFiClient networkClient;
    ESP8266WiFiClass wifiConnection;

};