#include <Arduino.h>

#include <U8g2lib.h>

#include <Wire.h>

#include "GPSTracker.h"
#include "Constants/Constants.h"

U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);

GPSTracker gpsTracker(GPS_LOG_PERIOD_SEC);

void showStatus() {
  u8g2.clearBuffer();					// clear the internal memory
  u8g2.setFont(u8g2_font_ncenB08_tr);	// choose a suitable font
  if (gpsTracker.getStatus()) {
    u8g2.drawStr(0,10, gpsTracker.getStatusStr().c_str());	// write something to the internal memory
    u8g2.drawStr(0,20, gpsTracker.getGpsStatusStr().c_str());	// write something to the internal memory
    u8g2.drawStr(0,30, gpsTracker.getGpsDataStr().c_str());	// write something to the internal memory
  }
  else u8g2.drawStr(0,20, "Initialization...");
  u8g2.sendBuffer();					// transfer internal memory to the display
}

void setup()
{
  //Serial.begin(9600);
  //while(!Serial) ;
  u8g2.begin();
  showStatus();
  gpsTracker.init();
}

void loop()
{
  showStatus();
  gpsTracker.readGpsData();
}



