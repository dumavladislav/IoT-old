#include "MQTTLightControl.h"
#include <MQTTClient.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <WifiConnect.h>
#include <MessageBuilder.h>
#include <MessageParser.h>
#include <DeviceSettings.h>

#include <iostream>
#include <unordered_map>

MQTTLightControl::MQTTLightControl(/*char *devId, */char *mqttServer, int mqttPort, uint32_t chipId)
{
  this->authorizationBlock.deviceId = deviceSettings.deviceId;
  this->authorizationBlock.chipId = chipId;
  WifiConnect* wifiConnect = new WifiConnect(WIFI_SSID, (char*) WIFI_PSSWD);
  wifiConnect->connect();

  Serial.println("MAC:");
  Serial.println(wifiConnect->getMacAddress());
  this->authorizationBlock.macAddress = wifiConnect->getMacAddress();
  this->authorizationBlock.ipAddress = wifiConnect->getIpAddress().toString();

  mqttClient = new MQTTClient((char*)(deviceSettings.deviceId.c_str()));
  mqttClient->init(mqttServer, mqttPort, wifiConnect->getNetworkClient());
  setState(LOW);
  setDefaultPresets();
  resetTimer();
  oled.init();
}

DeviceSettings MQTTLightControl::getSettings() {
  return this->deviceSettings;
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

/*    const size_t jsonSize = JSON_OBJECT_SIZE(8);

    DynamicJsonDocument jsonDoc(jsonSize);

    float startTimeOnOperationMode = millis();
    String msgId = (char *)((String)authorizationBlock.deviceId 
    + String("_") + String(random(0xffff), HEX) 
    + String("_") + (String)startTimeOnOperationMode
    ).c_str();

    jsonDoc["msgId"] = msgId;
    jsonDoc["chipId"] = this->authorizationBlock.chipId;
    jsonDoc["macAddress"] = this->authorizationBlock.macAddress;
    jsonDoc["ipAddress"] = this->authorizationBlock.ipAddress;


    String jsonSerialized;
    serializeJson(jsonDoc, jsonSerialized);

    serializeJsonPretty(jsonDoc, Serial);
    mqttClient->sendMessage(AUTHORIZATION_REQUESTS_TPC, (char*)jsonSerialized.c_str());
      // ... and resubscribe
    mqttClient->subscribe(AUTHORIZATION_REQUESTS_STATUS_TPC); */
    MessageBuilder messageBuilder(authorizationBlock);
    messageBuilder.addElement("requestType", "authorization");
    mqttClient->sendMessage(AUTHORIZATION_REQUESTS_TPC, messageBuilder.generateJson());
    mqttClient->subscribe(AUTHORIZATION_REQUESTS_STATUS_TPC);   
}   

void MQTTLightControl::updateState(int newState)
{
  switch (deviceSettings.operationMode)
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

operationModes MQTTLightControl::decodeOperationMode(int intVar)
{
  operationModes enumVar = static_cast<operationModes>(intVar);
}

int MQTTLightControl::getState()
{
  return MSState;
}

void MQTTLightControl::setState(int newState) {
  MSState = newState;
}

void MQTTLightControl::sendMSState()
{
  //char message[50];
  //snprintf(message, 50, "%d", MSState);
  MessageBuilder messageBuilder(authorizationBlock);
  messageBuilder.addElement("msState", String(getState()));
  mqttClient->sendMessage(MOTION_TPC, messageBuilder.generateJson());
}

// void MQTTLightControl::sendOperationMode()
// {
//   char message[50];
//   snprintf(message, 50, "%d", deviceSettings.operationMode);
//   mqttClient->sendMessage(DEVICE_SETTINGS_APPLY_TPC, message);
// }

void MQTTLightControl::msDrivenOperation(int newState)
{
  /*int newStateDigital = 0;
  if (newState >= 333)
    newStateDigital = HIGH;
  else
    newStateDigital = LOW;
  */

  if (getState() != newState)
  {
    // Serial.println((millis() - startTimeOfOperation)/1000.0);
    // Serial.println(deviceSettings.motionSensorSettings.msDriveModeSettings.keepOnDurationSec);
    // Serial.println();
    if(
      newState == HIGH 
      || 
      (newState == LOW 
        and ((millis() - startTimeOfOperation) >= (deviceSettings.motionSensorSettings.msDriveModeSettings.keepOnDurationSec * 1000))
      )
    ) {
      setState(newState);
      resetTimer();
      sendMSState();
    }
  }
}

void MQTTLightControl::offOperation()
{
  if (getState() == HIGH)
  {
    setState(LOW);
    sendMSState();
  }
}

void MQTTLightControl::onOperation()
{
  if (getState() == LOW)
  {
    resetTimer();
    setState(HIGH);
    sendMSState();
  }
  else
  {
    float currTime = millis();
    // Serial.println("====================================");
    // Serial.println((currTime - startTimeOfOperation));
    // Serial.println(deviceSettings.motionSensorSettings.onModeSettings.maxOnOperationModeDuration);
    // Serial.println("====================================");
    // Serial.println();
    if (((currTime - startTimeOfOperation)/1000) >= deviceSettings.motionSensorSettings.onModeSettings.maxOnOperationModeDuration)
      this->deviceSettings.operationMode = operationModes::MSDRIVEN;
  }
}

void MQTTLightControl::authorizationResponse(String message) {
  const size_t jsonSize = JSON_OBJECT_SIZE(6) + 2*JSON_OBJECT_SIZE(2) + 120;
  DynamicJsonDocument jsonDoc(jsonSize);

  deserializeJson(jsonDoc, message);

  if(String((const char*)jsonDoc["header"]["macAddress"]) == this->authorizationBlock.macAddress) {

    this->authorizationBlock.authorized = jsonDoc["data"]["authorized"];
    this->authorizationBlock.securityToken = (const char *)jsonDoc["data"]["sessionToken"];

    settingsRequest();
  }
}

void MQTTLightControl::settingsRequest() {

  MessageBuilder messageBuilder(authorizationBlock);
  messageBuilder.addElement("requestType", "settings");
  mqttClient->sendMessage(DEVICE_SETTINGS_REQUEST_TPC, messageBuilder.generateJson());
  mqttClient->subscribe(DEVICE_SETTINGS_APPLY_TPC);
}

void MQTTLightControl::applyNewSettings(String message) {
  Serial.println("New settings received!");
  MessageParser messageParser(authorizationBlock);
  if(messageParser.parseIncomingMessage(message.c_str())) {
    JsonObject data = messageParser.getDataBlock();
    DeviceSettings newSettings;
    newSettings.deviceId = (const char*) data["deviceId"];
    newSettings.operationMode = (operationModes)((int)data["operationMode"]);
    newSettings.motionSensorSettings.relayMode = data["motionSensorSettings"]["relayMode"];
    newSettings.motionSensorSettings.msDriveModeSettings.keepOnDurationSec = data["motionSensorSettings"]["msDriveModeSettings"]["keepOnDurationSec"];
    Serial.println("=======================================");
    Serial.println(String((const char*)data["motionSensorSettings"]["onModeSettings"]["maxOnOperationModeDuration"]));
    Serial.println("=======================================");
    newSettings.motionSensorSettings.onModeSettings.maxOnOperationModeDuration = data["motionSensorSettings"]["onModeSettings"]["maxOnOperationModeDuration"];
    this->setDeviceSettings(newSettings);
  }
}

void MQTTLightControl::setDeviceSettings(DeviceSettings newSettings) {
  this->deviceSettings = newSettings;
  resetTimer();
}

void MQTTLightControl::setDefaultPresets() {
  // for(int i=0; i<devicePresets.size(); i++)
  // this->devicePresets.
  devicePresets.assign(4, deviceSettings);

  devicePresets[0].operationMode = operationModes::MSDRIVEN;

  devicePresets[1].operationMode = operationModes::ON;

  devicePresets[2].operationMode = operationModes::OFF;
  
}

void MQTTLightControl::applyPreset(int presetNumber) {
  setDeviceSettings(devicePresets[presetNumber]);
}

void MQTTLightControl::increaseTimer() {
  switch(deviceSettings.operationMode) {
    case operationModes::MSDRIVEN:
      deviceSettings.motionSensorSettings.msDriveModeSettings.keepOnDurationSec += 5; 
      break;
    case operationModes::ON:
      deviceSettings.motionSensorSettings.onModeSettings.maxOnOperationModeDuration +=10;
      break;
  }
}

void MQTTLightControl::decreaseTimer() {
  switch(deviceSettings.operationMode) {
    case operationModes::MSDRIVEN:
      deviceSettings.motionSensorSettings.msDriveModeSettings.keepOnDurationSec -= 5; 
      break;
    case operationModes::ON:
      deviceSettings.motionSensorSettings.onModeSettings.maxOnOperationModeDuration -=10;
      break;
  }
}

void MQTTLightControl::resetTimer() {
  startTimeOfOperation = millis();
}

void MQTTLightControl::showStatus() {
  oled.clear();
  DeviceSettings settings = getSettings();
  String msStateStr = (MSState ? "ON " : "OFF");
  
  switch(settings.operationMode) {
    case operationModes::MSDRIVEN: {
      oled.addString("MS | " + msStateStr);
      if (MSState == 1) {
        oled.addString("Timer: "+ String(settings.motionSensorSettings.msDriveModeSettings.keepOnDurationSec) + " | Left: " + String(settings.motionSensorSettings.msDriveModeSettings.keepOnDurationSec - ((millis() - startTimeOfOperation)/1000),0));
      }
      break;
    }
    case operationModes::ON: {
      oled.addString("ON | " + msStateStr);
      if (MSState == 1) {
        oled.addString("Timer: "+ String(settings.motionSensorSettings.onModeSettings.maxOnOperationModeDuration) + " | Left: " + String(settings.motionSensorSettings.onModeSettings.maxOnOperationModeDuration - ((millis() - startTimeOfOperation)/1000),0));
      }
      break;
    }
    case operationModes::OFF: {
      oled.addString("OFF | " + msStateStr);
      break;
    }
  }
  if(settings.operationMode == operationModes::MSDRIVEN) {
      
  }
  oled.show();
}