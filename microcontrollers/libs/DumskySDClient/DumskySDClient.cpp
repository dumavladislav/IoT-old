#include <Arduino.h>
#include "DumskySDClient.h"

Dumsky::SDClient::SDClient() {

}

void Dumsky::SDClient::init() {
  Serial.print("Initializing SD card...");

  if (!SD.begin(5)) {
    Serial.println("SD initialization failed!");
    while (1);
  }
  Serial.println("SD initialization done.");


  
  if (SD.exists("/fcntr.txt")) {
    Serial.println("Reading fileCounter");
    File fileCounterFile = SD.open("/fcntr.txt");

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
    fileCounterFile = SD.open("/fcntr.txt", FILE_WRITE);
    fileCounterFile.println(filesCounter);
    fileCounterFile.close();
    if (SD.exists("/fcntr.txt")) Serial.println("fcntr.txt created");
  }
}

String Dumsky::SDClient::generateFileHeader() {
  String logStringTTT = "";
  logStringTTT += String("recNum");
  logStringTTT += String(",");
  logStringTTT += String("millis");
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

void Dumsky::SDClient::writeLogString(String logString) {

    recordsCounter++;
    recordsInFileCounter++;

    // Serial.print("WRITELOGSTRING: ");
    // Serial.println(logString);

    if(recordsInFileCounter > maxRecordsInFile) {
        filesCounter++;
        SD.remove("/fcntr.txt");
        fileCounterFile = SD.open("/fcntr.txt", FILE_WRITE);
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
      
      File dataFile = SD.open(String("/") + (String(deviceId)+String("_")+String(filesCounter) + String(".txt")).c_str(), FILE_WRITE);

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