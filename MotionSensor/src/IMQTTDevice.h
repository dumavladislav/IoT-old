class IMQTTDevice {
public:
    virtual void sendMessage(char* topicName, char message[]) = 0;
private:
    virtual void mqttConnect(char* mqttUsr, char* mqttPasswd) = 0;
};