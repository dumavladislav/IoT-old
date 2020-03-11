class IWifiDevice {
private:
    virtual void wifiConnect(char* ssid, char* password) = 0;
};