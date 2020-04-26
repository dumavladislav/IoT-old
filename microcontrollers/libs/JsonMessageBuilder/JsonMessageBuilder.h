#pragma once
#include <ArduinoJson.h>
#include <AuthorizationBlock.h>

class JsonMessageBuilder {

public:
    JsonMessageBuilder(AuthorizationBlock authorizationBlock);
    void addElement(String elementName, String elementValue);
    void addObject(String objectName, JsonObject jsonObject);
    void addArrayOfObjects(String arrayName, JsonObject jsonObject[]);
    String toString();

private:
    DynamicJsonDocument jsonDoc = DynamicJsonDocument(1024);
    JsonObject header;
    JsonObject data;

};