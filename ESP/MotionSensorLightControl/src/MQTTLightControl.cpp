#include "MQTTLightControl.h"
#include "MQTTClient.h"

MQTTLightControl::MQTTLightControl(char *devId, char *mqttServer, int mqttPort /*, MQTT_CALLBACK_SIGNATURE*/)
{
  mqttClient = new MQTTClient(devId, mqttServer, mqttPort);
  MSState = LOW;
}

void MQTTLightControl::setCallback(MQTT_CALLBACK_SIGNATURE)
{
  mqttClient->setCallback(callback);
}

void MQTTLightControl::connect()
{
  mqttClient->connect(WIFI_SSID, WIFI_PSSWD, MQTT_USER, MQTT_PSSWD);
}

void MQTTLightControl::keepAlive(char *mqttUsr, char *mqttPasswd)
{
  mqttClient->keepAlive(mqttUsr, mqttPasswd);
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
  mqttClient->sendMessage(MOTION_SENSOR_STATE_TPC, message);
}

void MQTTLightControl::sendOperationMode()
{
  char message[50];
  snprintf(message, 50, "%d", operationMode);
  mqttClient->sendMessage(DEVICE_OPERATION_MODE_TPC, message);
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
