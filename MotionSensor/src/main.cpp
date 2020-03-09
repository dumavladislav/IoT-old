#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <map>
#include <iostream>
#include <ctime>

#include "Credentials.h"
#include "Constants.h"

// Update these with values suitable for your network.

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;

char msg[50];

boolean MSPreviousState = 0;
boolean MSState = 0;
int MSValue = 0;


// PINS DECLARATION
const int RELAY_PIN = D1;
const int MS_PIN = A0;
//const int RELAY_SCAN = D3;

////////////

void sendMessage(char message[]) {
  // current date/time based on current system
  time_t now = time(0);
   
  // convert now to string form
  char* dt = ctime(&now);

  snprintf (msg, 50, "%s%s: %s", dt, MOTION_SENSOR_ID, message);
  client.publish((char* ) MOTION_SENSOR_STATE, msg);
}

void sendMSState(boolean currState) {
  char message[50];
  snprintf (message, 50, "%d", currState);
  sendMessage(message);
}

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PSSWD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

}

void reconnect(String clientId) {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(clientId.c_str(), MQTT_USER, MQTT_PSSWD)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(GREETING_TPC, (clientId + " connected").c_str());
      // ... and resubscribe
      //client.subscribe(LIGHT_STATE_TPC);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  pinMode(MS_PIN, INPUT);     // Initialize the BUILTIN_LED pin as an output
  pinMode(RELAY_PIN, OUTPUT);
//  pinMode(RELAY_SCAN, INPUT);
  
  digitalWrite(RELAY_PIN, HIGH);
  
  Serial.begin(9600);
  setup_wifi();
  client.setServer(MQTT_SERVER, 1883);
  client.setCallback(callback);  
}

void loop() {

  if (!client.connected()) {
    reconnect(MOTION_SENSOR_ID + String(random(0xffff), HEX));
  }
  client.loop();

  delay(10);
  MSValue = analogRead(MS_PIN);

  Serial.print("MSValue = ");
  Serial.println(MSValue);

  if (MSValue >= 333) MSState = HIGH;
  else MSState = LOW;

//  boolean RelayState = digitalRead(RELAY_SCAN);
//  Serial.println("MSState = " + MSState);


  if (MSState != MSPreviousState) {
    MSPreviousState = MSState;
    Serial.println("State changed!");
    digitalWrite(RELAY_PIN, !MSState);
    sendMSState(MSState);
    
  }

}

