#include <Arduino.h>
#include <vector>

#include <iostream>
#include <vector>
#include <string>
#include <sstream>  
 
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "Credentials.h"
#include "Constants.h"

// Update these with values suitable for your network.

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;

char msg[50];

boolean ledState = 0;
char ledColors[3];

// PINS DECLARATION
const int LED_PIN_R = D2;
const int LED_PIN_G = D3;
const int LED_PIN_B = D4;
////////////

using namespace std;
vector<string> split(string str, char delimiter) {
  vector<string> internal;
  stringstream ss(str); // Turn the string into a stream.
  string tok;
 
  while(getline(ss, tok, delimiter)) {
    internal.push_back(tok);
  }
 
  return internal;
}

char* getLedState(byte* payload) {
  //snprintf (msg, 50, "%d;%d;%d", ledColors[0], ledColors[1], ledColors[2]);
  //client.publish((char* ) LIGHT_TOGGLE_TPC, msg);

  vector<string> sep = split((char* )payload, ';');

  ledColors[0] = atoi(sep[0].c_str());
  ledColors[1] = atoi(sep[1].c_str());
  ledColors[2] = atoi(sep[2].c_str());

  /*if ((char)payload[0] == '1') {

      digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
      // but actually the LED is on; this is because
      // it is active low on the ESP-01)
    } else {
      digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
    }*/
}


void changeColors(char ledColors[3]) {
  digitalWrite(LED_PIN_R , ledColors[0]);
  digitalWrite(LED_PIN_G , ledColors[1]);
  digitalWrite(LED_PIN_B , ledColors[2]);
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
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  if(topic == LIGHT_TOGGLE_TPC) {
    // Switch on the LED if an 1 was received as first character

    changeColors(getLedState(payload));
  }
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
      client.subscribe(LIGHT_TOGGLE_TPC);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

/*boolean readButton() {

  int current = digitalRead(BUTTON_PIN);

  if(current != lastButton) {
    delay(5);
    current = digitalRead(BUTTON_PIN);
    return current;
  }
  return current;
}*/


void setup() {
  pinMode(LED_PIN_R, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  pinMode(LED_PIN_G, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  pinMode(LED_PIN_B, OUTPUT);     // Initialize the BUILTIN_LED pin as an output

  Serial.begin(9600);
  setup_wifi();
  client.setServer(MQTT_SERVER, 1883);
  client.setCallback(callback);

}

void loop() {

  if (!client.connected()) {
    reconnect("ButtonOwner-" + String(random(0xffff), HEX));
  }
  client.loop();

/*  long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;
    snprintf (msg, 50, "hello world #%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("outTopic", msg);
  }*/

/*  currentButton = readButton();
  if(lastButton == LOW && currentButton == HIGH) {
    
    Serial.print("Button pressed");
    ledOn = !ledOn;
    snprintf (msg, 50, "#%d", ledOn);
    client.publish("buttonTopic", msg);
  }
  lastButton = currentButton;*/

}

