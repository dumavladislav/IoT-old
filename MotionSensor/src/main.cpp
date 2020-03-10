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


// PINS DECLARATION
const int RELAY_PIN = D1;
const int MS_PIN = D2;

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

  //if(std::string::compare(topic, LIGHT_STATE_TPC)){
    
  //  sendLedState();
  //}


/*  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }*/

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
  MSState = digitalRead(MS_PIN);
//  Serial.print(MSState);


  if (MSState != MSPreviousState) {
    Serial.println("State changed!");
    digitalWrite(RELAY_PIN, !MSState);
    sendMSState(MSState);
    MSPreviousState = MSState;
  }        

}

