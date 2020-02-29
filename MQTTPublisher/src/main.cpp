#include <Arduino.h>

/*
 Basic ESP8266 MQTT example

 This sketch demonstrates the capabilities of the pubsub library in combination
 with the ESP8266 board/library.

 It connects to an MQTT server then:
  - publishes "hello world" to the topic "outTopic" every two seconds
  - subscribes to the topic "inTopic", printing out any messages
    it receives. NB - it assumes the received payloads are strings not binary
  - If the first character of the topic "inTopic" is an 1, switch ON the ESP Led,
    else switch it off

 It will reconnect to the server if the connection is lost using a blocking
 reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
 achieve the same result without blocking the main loop.

 To install the ESP8266 board, (using Arduino 1.6.4+):
  - Add the following 3rd party board manager under "File -> Preferences -> Additional Boards Manager URLs":
       http://arduino.esp8266.com/stable/package_esp8266com_index.json
  - Open the "Tools -> Board -> Board Manager" and click install for the ESP8266"
  - Select your ESP8266 in "Tools -> Board"

*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <map>
#include "GyverButton.h"

#include "Credentials.h"
#include "Constants.h"

// Update these with values suitable for your network.

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;

char msg[50];

char ledColors[3];

boolean ledState = 0;


// PINS DECLARATION
const int BUTTON_PIN = D1;
const int LED_PIN_R = D2;
const int LED_PIN_G = D3;
const int LED_PIN_B = D4;
////////////

GButton butt1(BUTTON_PIN);

void sendLedState() {
  snprintf (msg, 50, "%d;%d;%d", ledColors[0], ledColors[1], ledColors[2]);
  client.publish((char* ) LIGHT_TOGGLE_TPC, msg);
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

  if(topic == LIGHT_STATE_TPC){
    sendLedState();
  }


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
      client.subscribe(LIGHT_STATE_TPC);
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


  butt1.setDebounce(50);
  butt1.setTimeout(300);
//  butt1.setIncrStep(2);
//  butt1.setIncrTimeout(500);

  ledColors[0] = 255;
  ledColors[1] = 255;
  ledColors[2] = 255;

}

void changeColors() {
  std::srand(time(0));
  ledColors[0] = (float)(std::rand()%2) > 0.5 ? HIGH : LOW;
  ledColors[1] = (float)(std::rand()%2) > 0.5 ? HIGH : LOW;
  ledColors[2] = (float)(std::rand()%2) > 0.5 ? HIGH : LOW;

  snprintf (msg, 50, "%d;%d;%d", ledColors[0], ledColors[1], ledColors[2]);

  digitalWrite(LED_PIN_R , ledColors[0]);
  digitalWrite(LED_PIN_G , ledColors[1]);
  digitalWrite(LED_PIN_B , ledColors[2]);
}

void loop() {

  if (!client.connected()) {
    reconnect("ButtonOwner-" + String(random(0xffff), HEX));
  }
  client.loop();

  butt1.tick();

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

  if (butt1.isPress()) {
    Serial.println("Button pressed");
    ledState = !ledState;
    changeColors();
    sendLedState();
  }        

}

