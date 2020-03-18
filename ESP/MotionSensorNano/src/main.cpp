#include <Arduino.h>

// Update these with values suitable for your network.

boolean MSState = 0;
int MSValue = 0;


// PINS DECLARATION
const int RELAY_PIN = 4;
const int MS_PIN = A0;
const int RELAY_SCAN = 3;

////////////



void setup() {
  pinMode(MS_PIN, INPUT);     // Initialize the BUILTIN_LED pin as an output
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(RELAY_SCAN, INPUT);
  
  digitalWrite(RELAY_PIN, LOW);
  
  Serial.begin(9600);
}

void loop() {

  delay(10);
  MSValue = analogRead(MS_PIN);

  Serial.println("MSValue = " + MSValue);

  if (MSValue >= 333) MSState = HIGH;
  else MSState = LOW;

  boolean RelayState = digitalRead(RELAY_SCAN);
  Serial.println("MSState = " + MSState);


  if (MSState != RelayState) {
    Serial.println("State changed!");
    digitalWrite(RELAY_PIN, MSState);
  }        

}

