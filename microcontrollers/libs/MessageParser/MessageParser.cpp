#include "MessageParser.h"

MessageParser::MessageParser(AuthorizationBlock deviceAuthorizationBlock) {
    this->deviceAuthorizationBlock = deviceAuthorizationBlock;
}

bool MessageParser::parseIncomingMessage(std::string incomingMessage) {

    const size_t jsonSize = 2*JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(2) + 2*JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(5) + 330;
    DynamicJsonDocument jsonDoc(jsonSize);

    deserializeJson(jsonDoc, incomingMessage);
    //serializeJsonPretty(&jsonDoc, Serial);
    if(jsonDoc["header"]["macAddress"] == deviceAuthorizationBlock.macAddress) {
        this->header = jsonDoc["header"];
        this->data = jsonDoc["data"];
        return true;
    }
    else return false;
}

AuthorizationBlock MessageParser::getAuthorizationBlock() {
    AuthorizationBlock authBlock;
    authBlock.authorized = this->header["authorized"];
    authBlock.chipId = this->header["chipId"];
    authBlock.deviceId = (const char*)this->header["deviceId"];
    authBlock.ipAddress = (const char*)this->header["ipAddress"];
    authBlock.macAddress = (const char*)this->header["macAddress"];
    authBlock.securityToken = (const char*)this->header["securityToken"];
    return authBlock;
}

JsonObject MessageParser::getDataBlock() {
    return this->data;
}