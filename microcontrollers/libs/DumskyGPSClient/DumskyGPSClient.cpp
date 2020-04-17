#include <Arduino.h>
#include "DumskyGPSClient.h"

GPSClient::GPSClient() {
}

GPSClient::~GPSClient() {
  Serial.print("DESTRUCTOR: ");
  Serial.println("GPSClient");
}

void GPSClient::init() {
    Serial.println(F("A simple demonstration of TinyGPS++ with an attached GPS module"));
    Serial.print(F("Testing TinyGPS++ library v. ")); Serial.println(TinyGPSPlus::libraryVersion());
    Serial.println(F("by Mikal Hart"));
    Serial.println();

    //ss(RXPin, TXPin);
    ss = new SoftwareSerial(RXPin, TXPin);
    ss->begin(GPSBaud);
    gpsStartTime = millis();
}

GpsData GPSClient::readGpsData() {
  
  GpsData gpsData;
  // Serial.println("Reading GPS Data");
  // This sketch displays information every time a new sentence is correctly encoded.

  while (ss->available())
  {
    if (gps.encode(ss->read()))
    {
      if (gps.location.isValid())
      {
        gpsData.gpsDataValid = gps.location.isValid();
        gpsData.lat =  gps.location.lat();
        gpsData.lng =  gps.location.lng();
        if (gps.date.isValid())
        {
          gpsData.gpsDateValid = true;
          gpsData.gpsDay = gps.date.day();
          gpsData.gpsMonth = gps.date.month();
          gpsData.gpsYear = gps.date.year();
          //logString += String(day()) + String(F("/")) + String(month()) + String(F("/")) + String(year());
        }

        if (gps.time.isValid())
        {
          gpsData.gpsTimeValid = true;
          gpsData.gpsHour = gps.time.hour();
          gpsData.gpsMinute = gps.time.minute();
          gpsData.gpsSecond = gps.time.second();
          gpsData.gpsCentisecond = gps.time.centisecond();
        }
      
      }
    }
  }

  if (millis() > 15000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  } 

  return gpsData;
}


String GpsData::toString() {
  String logString = "";

  if (lat != 0 && lng != 0 && lat >= -90 && lat <= 90 && lng >= -180 && lng <= 180)
  {
    
    logString += String(millis());
    logString += String(",");
  
    logString += String(lat, 6) + String(",") + String(lng, 6);

    logString += String(F(","));
  
    if (gpsDateValid)
    {
      logString += String(gpsYear) + String(F(".")) + String(gpsMonth) + String(F(".")) + String(gpsDay);
    }
    else
    {
      logString += String("1970") + String(F(".")) + String("01") + String(F(".")) + String("01");
    }

    logString += String(F(","));
    if (gpsTimeValid)
    {
      if (gpsHour < 10) logString += String(F("0")); //Serial.print(F("0"));
      logString += String(gpsHour) + String(F(":"));
      if (gpsMinute < 10) logString += String(F("0")); // Serial.print(F("0"));
      logString += String(gpsMinute) + String(F(":"));
      if (gpsSecond < 10) logString += String(F("0")); // Serial.print(F("0"));
      logString += String(gpsSecond) + String(F("."));
      if (gpsCentisecond < 10) logString += String(F("0")); // Serial.print(F("0"));
      logString += String(gpsCentisecond);
    }
    else
    {
      logString += String(F("00:00:00.00"));
    }
  
  }

  return logString;
}

