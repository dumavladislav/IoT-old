#include "GPSTracker.h"

GPSTracker::GPSTracker(int gpsScanPeriod) {
    this->gpsScanPeriod = gpsScanPeriod;
}

void GPSTracker::init() {
    //gsmConnect.init();
    gpsClient.init();
    sdClient.init();
    //mqttClient = new MQTTClient((char*)"1111", "123231", 1883, (Client*) gsmConnect.getClient());
}

void GPSTracker::readGpsData() {
    if ((millis() - lastGpsScanTime) > (gpsScanPeriod*1000)) {
        GpsData gpsData = gpsClient.readGpsData();
        if (gpsData.gpsDataValid && gpsData.lat > 0 && gpsData.lng > 0) {
            Serial.println("VALID DATA");
            sdClient.writeLogString(gpsData.toString());
        }
        else {
            Serial.print("Invalid Data.");
        }
        lastGpsScanTime = millis();
    }
}