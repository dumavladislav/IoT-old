#include <Arduino.h>

#include <TinyGPS++.h>
#include <SoftwareSerial.h>

#include <SPI.h>
#include <SD.h>

/*
   This sample sketch demonstrates the normal use of a TinyGPS++ (TinyGPSPlus) object.
   It requires the use of SoftwareSerial, and assumes that you have a
   4800-baud serial GPS device hooked up on pins 4(rx) and 3(tx).
*/
static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 9600;

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

int filesCounter = 0;
int recordsCounter = 0;
int recordsInFileCounter = 0;

const int maxRecordsInFile = 10000;

File fileCounterFile;

const String deviceId = "GPS1";

void setup()
{
  Serial.begin(9600);
  ss.begin(GPSBaud);

  Serial.println(F("DeviceExample.ino"));
  Serial.println(F("A simple demonstration of TinyGPS++ with an attached GPS module"));
  Serial.print(F("Testing TinyGPS++ library v. ")); Serial.println(TinyGPSPlus::libraryVersion());
  Serial.println(F("by Mikal Hart"));
  Serial.println();

  Serial.print("Initializing SD card...");

  if (!SD.begin(10)) {
    Serial.println("SD initialization failed!");
    while (1);
  }
  Serial.println("SD initialization done.");


  
  if (SD.exists("fcntr.txt")) {
    Serial.println("Reading fileCounter");
    File fileCounterFile = SD.open("fcntr.txt");

    // if the file is available, write to it:
    if (fileCounterFile) {
      int i = 0;
      byte fileContent[20];
      while (fileCounterFile.available()) {
        fileContent[i] = fileCounterFile.read();
        i++;
        //Serial.write(dataFile.read());
      }
      fileCounterFile.close();
      if(i > 0) filesCounter = atoi((char*)fileContent);
      Serial.println("Files Counter:");
      Serial.println(filesCounter);
    }
    // if the file isn't open, pop up an error:
    else {
      Serial.println("error opening fcntr.txt");
    }
  }
  else { 
    Serial.println("Creating fcntr");
    filesCounter = 0;
    fileCounterFile = SD.open("fcntr.txt", FILE_WRITE);
    fileCounterFile.println(filesCounter);
    fileCounterFile.close();
    if (SD.exists("fcntr.txt")) Serial.println("fcntr.txt created");
  }
}

String generateFileHeader() {
  String logStringTTT = "";
  logStringTTT += String("recNum");
  logStringTTT += String(",");
  logStringTTT += String(millis());
  logStringTTT += String(",");
  logStringTTT += String("lat ");
  logStringTTT += String(",");
  logStringTTT += String("lng ");
  logStringTTT += String(",");
  logStringTTT += String("date ");
  logStringTTT += String(",");
  logStringTTT += String("time ");
  return logStringTTT;
}

String getlogString()
{
  String logStringTTT = "";
  logStringTTT += String(recordsCounter);
  logStringTTT += String(",");
  logStringTTT += String(millis());
  logStringTTT += String(",");
  //logStringTTT += String(F("Location: "));
  if (gps.location.isValid())
  {
    logStringTTT += String(gps.location.lat(), 6) + String(",") + String(gps.location.lng(),6);
  }
  else
  {
    logStringTTT += String(F("00.000000")) + String(",") + String(F("00.000000"));
  }

  //logStringTTT += String(F("  Date/Time: "));
   logStringTTT += String(F(","));
 
  if (gps.date.isValid())
  {
    logStringTTT += String(gps.date.year()) + String(F(".")) + String(gps.date.month()) + String(F(".")) + String(gps.date.day());
    //logStringTTT += String(day()) + String(F("/")) + String(month()) + String(F("/")) + String(year());
  }
  else
  {
    logStringTTT += String("1970") + String(F(".")) + String("01") + String(F(".")) + String("01");
  }

  logStringTTT += String(F(","));
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) logStringTTT += String(F("0")); //Serial.print(F("0"));
    logStringTTT += String(gps.time.hour()) + String(F(":"));
    // logStringTTT += String(hour()) + String(F(":"));
    if (gps.time.minute() < 10) logStringTTT += String(F("0")); // Serial.print(F("0"));
    logStringTTT += String(gps.time.minute()) + String(F(":"));
    //logStringTTT += String(minute()) + String(F(":"));
    if (gps.time.second() < 10) logStringTTT += String(F("0")); // Serial.print(F("0"));
    logStringTTT += String(gps.time.second()) + String(F("."));
    //logStringTTT += String(second()) + String(F(":"));
    if (gps.time.centisecond() < 10) logStringTTT += String(F("0")); // Serial.print(F("0"));
    logStringTTT += String(gps.time.centisecond());
  }
  else
  {
    logStringTTT += String(F("00:00:00.00"));
  }
  

  // Serial.println(logStringTTT);
  return logStringTTT;
}

void loop()
{
  // This sketch displays information every time a new sentence is correctly encoded.
  while (ss.available() > 0)
    if (gps.encode(ss.read()))
    {
      recordsCounter++;
      recordsInFileCounter++;

      String logString = getlogString();
      if(recordsInFileCounter > maxRecordsInFile) {
        filesCounter++;
        SD.remove("fcntr.txt");
        fileCounterFile = SD.open("fcntr.txt", FILE_WRITE);
        if (fileCounterFile) {
          fileCounterFile.print(filesCounter);
          fileCounterFile.close();
          Serial.println("Files COUNTER updated!!!");
        }
        // if the file isn't open, pop up an error:
        else {
          Serial.println("error opening fcntr.txt");
        }
        recordsInFileCounter = 1;
      }
      
      File dataFile = SD.open((String(deviceId)+String("_")+String(filesCounter) + String(".txt")).c_str(), FILE_WRITE);

      // if the file is available, write to it:
      if (dataFile) {
        if(recordsInFileCounter == 1) 
        { 
          Serial.println(generateFileHeader());
          dataFile.println(generateFileHeader());
        }

        dataFile.println(logString);
        dataFile.close();
        // print to the serial port too:
        Serial.println(logString);
      }
      // if the file isn't open, pop up an error:
      else {
        Serial.println("error opening datalog.txt");
      }
    }

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }
}

