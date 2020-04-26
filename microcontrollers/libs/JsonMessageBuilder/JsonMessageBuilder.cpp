#include "JsonMessageBuilder.h"

JsonMessageBuilder::JsonMessageBuilder(AuthorizationBlock authorizationBlock) {
    // const size_t jsonSize = JSON_OBJECT_SIZE(hashmap.size()) + JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(4) + 30*(hashmap.size()+10);
    // const size_t jsonSize = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(4);
    // Serial.println(2*JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(5) + 100);
    // jsonDoc = new DynamicJsonDocument(2*JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(5));

    // // HEADER

    String msgId = (char *)((String)authorizationBlock.deviceId 
    + String("_") + String(random(0xffff), HEX) 
    + String("_") + (String)millis()
    ).c_str();

    header = jsonDoc.createNestedObject("header");


    header["msgId"] = msgId;
    header["chipId"] = authorizationBlock.chipId;
    header["macAddress"] = authorizationBlock.macAddress;
    header["ipAddress"] = authorizationBlock.ipAddress;
    header["secToken"] = authorizationBlock.securityToken;

    //JsonObject root = jsonDoc->to<JsonObject>();

    data = jsonDoc.createNestedObject("data");
    //data["qwe11"] = 222;

    String jsonSerialized;
    serializeJson(jsonDoc, jsonSerialized);
    // Serial.println(jsonSerialized);
}

void JsonMessageBuilder::addElement(String elementName, String elementValue) {
    data[elementName] = elementValue;
}

void JsonMessageBuilder::addObject(String objectName, JsonObject jsonObject) {
    JsonObject newObj = data.createNestedObject(objectName);
    for (JsonObject::iterator it=jsonObject.begin(); it!=jsonObject.end(); ++it)
    {
        // Serial.print(it->key().c_str());
        // Serial.print(" = ");
        // Serial.println(it->value().as<char*>());
        newObj[it->key().c_str()] = it->value().as<char*>();
    }
    // newObj = jsonObject;
}

void JsonMessageBuilder::addArrayOfObjects(String arrayName, JsonObject jsonObjects[]) {
    JsonArray arr = data.createNestedArray(arrayName);
    for(int i=0; i < (sizeof(jsonObjects)/sizeof(jsonObjects[0])); i++) {
    //for (JsonObject obj : jsonObjects) {
        arr.add(jsonObjects[i]);
    }
}

String JsonMessageBuilder::toString() {


    String jsonSerialized;
    serializeJson(jsonDoc, jsonSerialized);

    //serializeJsonPretty(*jsonDoc, Serial);

    return jsonSerialized;  
}