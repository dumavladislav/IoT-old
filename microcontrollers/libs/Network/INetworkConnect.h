#pragma once
#include <Client.h>

class INetworkConnect {

public:
    virtual Client* connect() = 0;
    virtual Client* getNetworkClient() = 0;
    virtual IPAddress getIpAddress() = 0;
    virtual String getMacAddress() = 0;
    virtual bool isConnected() = 0;
    virtual void disconnect() = 0;

};