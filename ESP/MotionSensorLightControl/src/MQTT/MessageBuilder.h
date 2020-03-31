#include <iostream>
#include <unordered_map>
#include <ArduinoJson.h>
#include "../AuthorizationBlock.h"

class MessageBuilder {

public:
    MessageBuilder(AuthorizationBlock authorizationBlock);
    void addElement(String key, String value);
    String generateJson();


private:
    AuthorizationBlock authorizationBlock;
    std::unordered_map<std::string, std::string> hashmap;

    

};