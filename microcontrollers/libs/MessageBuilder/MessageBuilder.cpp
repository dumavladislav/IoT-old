

#pragma once
#include "MessageBuilder.h"
#include <ArduinoJson.h>
// #include <iostream>
// #include <unordered_map>
//#include <map>
#include <AuthorizationBlock.h>

MessageBuilder::MessageBuilder(AuthorizationBlock authorizationBlock) {
  this->authorizationBlock = authorizationBlock;
}

void MessageBuilder::addElement(String key, String value) {
  


  hashmap[key.c_str()] = value.c_str();
}

String MessageBuilder::generateJson() {
    
    const size_t jsonSize = JSON_OBJECT_SIZE(hashmap.size()) + JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(4) + 30*(hashmap.size()+10);
    DynamicJsonDocument jsonDoc(jsonSize);

    // HEADER

    float startTimeOnOperationMode = millis();
    String msgId = (char *)((String)authorizationBlock.deviceId 
    + String("_") + String(random(0xffff), HEX) 
    + String("_") + (String)startTimeOnOperationMode
    ).c_str();

    JsonObject header = jsonDoc.createNestedObject("header");
    header["msgId"] = msgId;
    header["chipId"] = this->authorizationBlock.chipId;
    header["macAddress"] = this->authorizationBlock.macAddress;
    header["ipAddress"] = this->authorizationBlock.ipAddress;
    header["securityToken"] = this->authorizationBlock.securityToken;

    // JsonArray data = jsonDoc.createNestedArray("data");
    // data.add(48.756080);
    // data.add(2.302038);

    // Generate the minified JSON and send it to the Serial port.
    //

    if(hashmap.size() > 0) {

      JsonObject data = jsonDoc.createNestedObject("data");

      for(std::pair<std::string, std::string> element : hashmap ) {
        data[element.first.c_str()] = element.second.c_str();
      }
    }

    String jsonSerialized;
    //serializeJson(jsonDoc, Serial);
    serializeJson(jsonDoc, jsonSerialized);

    // Start a new line
    //Serial.println();

    // Generate the prettified JSON and send it to the Serial port.
    //
    //serializeJsonPretty(jsonDoc, Serial);

  return jsonSerialized;

}