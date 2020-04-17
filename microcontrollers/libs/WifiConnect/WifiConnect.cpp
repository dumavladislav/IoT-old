#include <Arduino.h>
#include "WifiConnect.h"
#include <ESP8266WiFi.h>

WifiConnect::WifiConnect(char *ssid, char *password) {
    this->ssid = ssid;
    this->password = password;
    // WiFiClient espClient;
    // this->networkClient = WiFiClient();
};

WiFiClient* WifiConnect::getNetworkClient() {
    return &this->networkClient;
}

IPAddress WifiConnect::getIpAddress() {
    return this->wifiConnection.localIP();
};

String WifiConnect::getMacAddress() {
    return this->wifiConnection.macAddress();
};

WiFiClient* WifiConnect::connect() {
    delay(10);
    // We start by connecting to a WiFi network
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    this->wifiConnection.begin(ssid, password);

    while (this->wifiConnection.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    randomSeed(micros());

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(this->wifiConnection.localIP());

    return &networkClient;
};

bool WifiConnect::isConnected() {
    if (this->wifiConnection.status() != WL_CONNECTED) return false;
    else return true;
}

void WifiConnect::disconnect() {
    this->wifiConnection.disconnect();
}
