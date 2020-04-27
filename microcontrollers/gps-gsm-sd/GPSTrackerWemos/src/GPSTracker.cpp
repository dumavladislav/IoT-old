#include "Constants/Credentials.h"
#include "GPSTracker.h"
#include <JsonMessageBuilder.h>

GPSTracker::GPSTracker(int gpsScanPeriod) {
    this->gpsScanPeriod = gpsScanPeriod;
    authorizationBlock.deviceId = DEVICE_ID;
    authorizationBlock.chipId = ESP.getChipId();
    gpsClient = new GPSClient(new SoftwareSerial(GPS_RX_PIN, GPS_TX_PIN));
}

void GPSTracker::init() {
    
    gsmConnect.init(GSM_APN, GSM_GPRS_USER, GSM_GPRS_PASS);
    authorizationBlock.macAddress = gsmConnect.getIMEI();
    // sdClient.init();
    mqttClient = new MQTTClient((char*)authorizationBlock.deviceId.c_str(), MQTT_SERVER, MQTT_PORT, (Client*) gsmConnect.getClient());
    mqttClient->keepAlive(MQTT_USER, MQTT_PSSWD);
    // mqttClient->authorizationRequest();
    gpsClient->init(/*GPS_RX_PIN, GPS_TX_PIN*/);
    status = 1;
}

uint8_t GPSTracker::getStatus() {
    return status;
}


void GPSTracker::readGpsData() {
    GpsData gpsData = gpsClient->readGpsData();
    // gsmConnect.forceListen();
    mqttClient->keepAlive(MQTT_USER, MQTT_PSSWD);
    if (((millis() - lastGpsScanTime) > (gpsScanPeriod*1000)) && gpsData.lat > 10 && gpsData.lng > 10) {
        if(prevGpsData.lat == 0) prevGpsData = gpsData;
        if (gpsData.lat != 0 
        && gpsData.lng != 0 
        && gpsData.lat >= -90 
        && gpsData.lat <= 90 
        && gpsData.lng >= -180 
        && gpsData.lng <= 180
        && 
        (
            ((gpsData.lat - prevGpsData.lat) <= 0.5 && (gpsData.lat - prevGpsData.lat) >= -0.5)
            && ((gpsData.lng - prevGpsData.lng) <= 0.5 && (gpsData.lng - prevGpsData.lng) >= -0.5)
            || millis() < 10000
            || coordCounter < 5
        ) 
        ) {
            if(mqttClient->sendMessage(GPS_TPC, getGpsDataJson(gpsData))) coordSentCounter++;
            prevGpsData = gpsData;
            coordCounter++;
            lastGpsScanTime = millis();
        }
    }
        
}

String GPSTracker::getGpsDataJson(GpsData gpsData) {
    
    JsonMessageBuilder jsonMessageBuilder(authorizationBlock);

    jsonMessageBuilder.addElement("millis", String(millis()));
    jsonMessageBuilder.addElement("lat", String(gpsData.lat, 6));
    jsonMessageBuilder.addElement("lng", String(gpsData.lng, 6));

    return jsonMessageBuilder.toString();
}

unsigned long GPSTracker::getSentCounter() {
    return coordSentCounter;
}


String GPSTracker::getStatusStr() {
    String status = "MQTT: ";
    if (mqttClient->isConnected()) status += "ON";
    else 
        status += "OFF";
    status += " | Sent: " + String(getSentCounter());
    return status;
}

String GPSTracker::getGpsStatusStr() {
    String status = "Sats: ";
    status += gpsClient->getNumberOfSatellites();
    status += " | " + String(lastGpsScanTime/1000) + " : " + String(millis()/1000);
    return status;
}

String GPSTracker::getGpsDataStr() {
    String status = String(prevGpsData.lat,6) + " : " + String(prevGpsData.lng,6);
    return status;
}