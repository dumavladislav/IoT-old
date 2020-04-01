#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>

///////////////////////// CUSTOM INCLUDES //////////////////////////////////
#include "MQTTLightControl.h"
#include "./Constants/Credentials.h"
//#include "./Constants/Constants.h"

///////////////////////// CUSTOM INCLUDES //////////////////////////////////

const char *ssid = WIFI_SSID;
const char *password = WIFI_PSSWD;

///////////////////////// CUSTOM CODE ///////////////////////////////////////

char msg[50];
MQTTLightControl *mqttLightControl;
int MSValue = 0;

// PINS DECLARATION
const int RELAY_PIN = D1;
const int MS_PIN = A0;
////////////


void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  String messageTemp;

  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
    messageTemp += (char)payload[i];
  }
  Serial.println();

  if (String(topic) == AUTHORIZATION_REQUESTS_STATUS_TPC)
  {
    mqttLightControl->authorizationResponse(messageTemp);
  }

  if (String(topic) == DEVICE_SETTINGS_APPLY_TPC)
  {
    mqttLightControl->applyNewSettings(messageTemp);
  }
  Serial.println();
}

///////////////////////// CUSTOM CODE ///////////////////////////////////////


void setup()
{
  Serial.begin(115200);
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  // ArduinoOTA.setHostname("myesp8266");

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
    {
      type = "sketch";
    }
    else
    { // U_SPIFFS
      type = "filesystem";
    }

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR)
    {
      Serial.println("Auth Failed");
    }
    else if (error == OTA_BEGIN_ERROR)
    {
      Serial.println("Begin Failed");
    }
    else if (error == OTA_CONNECT_ERROR)
    {
      Serial.println("Connect Failed");
    }
    else if (error == OTA_RECEIVE_ERROR)
    {
      Serial.println("Receive Failed");
    }
    else if (error == OTA_END_ERROR)
    {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  ///////////////////////// CUSTOM CODE ///////////////////////////////////////

  pinMode(MS_PIN, INPUT); // Initialize the BUILTIN_LED pin as an output
  pinMode(RELAY_PIN, OUTPUT);

  //  Serial.begin(9600);

  mqttLightControl = new MQTTLightControl(
  //  (char *)MOTION_SENSOR_ID, 
    (char *)MQTT_SERVER, 1883,  
    ESP.getChipId());
  mqttLightControl->setCallback(callback);
  mqttLightControl->connect();
  mqttLightControl->authorizationRequest();

  ///////////////////////// CUSTOM CODE ///////////////////////////////////////
}

void loop()
{
  ArduinoOTA.handle();

  ///////////////////////// CUSTOM CODE ///////////////////////////////////////

  mqttLightControl->keepAlive(MQTT_USER, MQTT_PSSWD);
  delay(10);
  mqttLightControl->updateState(analogRead(MS_PIN));
  //  Serial.println(analogRead(MS_PIN));
  if (mqttLightControl->getSettings().motionSensorSettings.relayMode)
    digitalWrite(RELAY_PIN, mqttLightControl->getState());
  else
    digitalWrite(RELAY_PIN, !(mqttLightControl->getState()));
  ///////////////////////// CUSTOM CODE ///////////////////////////////////////
}