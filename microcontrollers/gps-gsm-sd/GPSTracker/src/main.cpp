#include <Arduino.h>

#include "GPSTracker.h"

GPSTracker gpsTracker(5);

void setup()
{
  Serial.begin(9600);
  gpsTracker.init();
}

void loop()
{
  // This sketch displays information every time a new sentence is correctly encoded.

  gpsTracker.readGpsData();
}

