#include "MQTTLightControl.h"
#include "MQTTClient.h"
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include "WifiConnect.h"

MQTTLightControl::MQTTLightControl(char *devId, char *mqttServer, int mqttPort, uint32_t chipId)
{
  this->deviceId = devId;
  this->chipId = chipId;
  WifiConnect* wifiConnect = new WifiConnect(WIFI_SSID, (char*) WIFI_PSSWD);
  wifiConnect->connect();

  Serial.println("MAC:");
  Serial.println(wifiConnect->getMacAddress());
  this->macAddress = wifiConnect->getMacAddress();
  this->ipAddress = wifiConnect->getIpAddress().toString();

  mqttClient = new MQTTClient(devId, mqttServer, mqttPort, wifiConnect->getNetworkClient());
  MSState = LOW;
}

void MQTTLightControl::setCallback(MQTT_CALLBACK_SIGNATURE)
{
  mqttClient->setCallback(callback);
}

void MQTTLightControl::connect()
{
  mqttClient->connect(MQTT_USER, MQTT_PSSWD);
}

void MQTTLightControl::keepAlive(char *mqttUsr, char *mqttPasswd)
{
  mqttClient->keepAlive(mqttUsr, mqttPasswd);
}

void MQTTLightControl::authorizationRequest() {

    /*

    {
        "msgId": ...
        "chipId": ...
        "macAddress": ...
        "ipAddress": ...
    }

    */  

    const size_t jsonSize = JSON_OBJECT_SIZE(8);

    DynamicJsonDocument jsonDoc(jsonSize);

    startTimeOnOperationMode = millis();
    String msgId = (char *)((String)deviceId 
    + String("_") + String(random(0xffff), HEX) 
    + String("_") + (String)startTimeOnOperationMode
    ).c_str();

    jsonDoc["msgId"] = msgId;
    jsonDoc["chipId"] = this->chipId;
    jsonDoc["macAddress"] = this->macAddress;
    jsonDoc["ipAddress"] = this->ipAddress;

    // JsonArray data = jsonDoc.createNestedArray("data");
    // data.add(48.756080);
    // data.add(2.302038);

    // Generate the minified JSON and send it to the Serial port.
    //

    String jsonSerialized;
    //serializeJson(jsonDoc, Serial);
    serializeJson(jsonDoc, jsonSerialized);

    // Start a new line
    //Serial.println();

    // Generate the prettified JSON and send it to the Serial port.
    //
    serializeJsonPretty(jsonDoc, Serial);
    mqttClient->sendMessage(AUTHORIZATION_REQUESTS_TPC, (char*)jsonSerialized.c_str());
      // ... and resubscribe
    mqttClient->subscribe(AUTHORIZATION_REQUESTS_STATUS_TPC);    
}   

void MQTTLightControl::updateState(int newState)
{
  switch (getOperationMode())
  {
  case operationModes::MSDRIVEN:
    msDrivenOperation(newState);
    break;
  case operationModes::ON:
    onOperation();
    break;
  case operationModes::OFF:
    offOperation();
    break;
  default:
    break;
  }
}

operationModes MQTTLightControl::getOperationMode()
{
  return operationMode;
}

void MQTTLightControl::setOperationMode(int mode)
{
  Serial.println(mode);
  operationMode = decodeOperationMode(mode);
  sendOperationMode();
}

void MQTTLightControl::sendMSState(boolean currState){

};

operationModes MQTTLightControl::decodeOperationMode(int intVar)
{
  operationModes enumVar = static_cast<operationModes>(intVar);
}

int MQTTLightControl::getState()
{
  return MSState;
}

void MQTTLightControl::sendMSState()
{
  char message[50];
  snprintf(message, 50, "%d", MSState);
  mqttClient->sendMessage(MOTION_TPC, message);
}

void MQTTLightControl::sendOperationMode()
{
  char message[50];
  snprintf(message, 50, "%d", operationMode);
  mqttClient->sendMessage(UPDATE_DEVICE_SETTINGS_TPC, message);
}

void MQTTLightControl::msDrivenOperation(int newState)
{
  int newStateDigital = 0;
  if (newState >= 333)
    newStateDigital = HIGH;
  else
    newStateDigital = LOW;

  if (MSState != newStateDigital)
  {
    MSState = newStateDigital;
    sendMSState();
  }
}

void MQTTLightControl::offOperation()
{
  if (MSState == HIGH)
  {
    MSState = LOW;
    sendMSState(MSState);
  }
}

void MQTTLightControl::onOperation()
{
  if (MSState == LOW)
  {
    startTimeOnOperationMode = millis();
    MSState = HIGH;
    sendMSState(MSState);
  }
  else
  {
    float currTime = millis();
    if ((currTime - startTimeOnOperationMode) >= MAX_ON_OPERATION_MODE_DURATION_MS)
      offOperation();
  }
}

void MQTTLightControl::authorizationResponse(String message) {
  const size_t jsonSize = JSON_OBJECT_SIZE(4) + 120;
  DynamicJsonDocument jsonDoc(jsonSize);

  deserializeJson(jsonDoc, message);

  if(String((const char*)jsonDoc["macAddress"]) == this->macAddress) {

    this->authorized = jsonDoc["authorized"];
    this->securityToken = (const char *)jsonDoc["token"];


    Serial.println(authorized);
    Serial.println(securityToken);
  }
}