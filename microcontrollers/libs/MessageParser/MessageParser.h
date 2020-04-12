#include <iostream>
#include <ArduinoJson.h>
#include <AuthorizationBlock.h>

class MessageParser {

public:
    MessageParser(AuthorizationBlock deviceAuthorizationBlock);
    bool parseIncomingMessage(std::string incomingMessage);
    AuthorizationBlock getAuthorizationBlock();
    JsonObject getDataBlock();

private:
    AuthorizationBlock deviceAuthorizationBlock;
    JsonObject header;
    JsonObject data;



};