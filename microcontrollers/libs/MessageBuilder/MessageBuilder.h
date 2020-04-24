#pragma once
#include <iostream>
#include <unordered_map>
// #include <StandardCplusplus.h>
// #include <StandardCplusplus.h>
// #include <serstream>
// #include <string>
// #include <vector>
// #include <iterator>
// #include <map>

#include <ArduinoJson.h>
#include <AuthorizationBlock.h>

using namespace std;

// namespace std
// {
//   ohserialstream cout(Serial);
// }

class MessageBuilder {

public:
    MessageBuilder(AuthorizationBlock authorizationBlock);
    void addElement(String key, String value);
    String generateJson();


private:
    AuthorizationBlock authorizationBlock;
    std::unordered_map<std::string, std::string> hashmap;
    // std::map<std::string, std::string> hashmap;


    

};