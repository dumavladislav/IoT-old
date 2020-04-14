#include "GSMConnect.h"

Dumsky::GSMConnect::GSMConnect() {
    SoftwareSerial SerialAT(2, 3); // RX, TX
}

void Dumsky::GSMConnect::init() {
    modem = new TinyGsm(Serial);
    SerialMon.println("Initializing modem->..");
    modem->restart();
    // modem->init();

    String modemInfo = modem->getModemInfo();
    SerialMon.print("Modem Info: ");
    SerialMon.println(modemInfo);

    modem->gprsConnect(apn, gprsUser, gprsPass);

    SerialMon.print("Waiting for network...");
    if (!modem->waitForNetwork()) {
        SerialMon.println(" fail");
        delay(10000);
        return;
    }
    SerialMon.println(" success");

    if (modem->isNetworkConnected()) {
        SerialMon.println("Network connected");
    }

    // GPRS connection parameters are usually set after network registration
    SerialMon.print(F("Connecting to "));
    SerialMon.print(apn);
    if (!modem->gprsConnect(apn, gprsUser, gprsPass)) {
    SerialMon.println(" fail");
    delay(10000);
    return;
    }
    SerialMon.println(" success");

    if (modem->isGprsConnected()) {
        SerialMon.println("GPRS connected");
    }

    client = new TinyGsmClient(*modem);
}

TinyGsmClient* Dumsky::GSMConnect::getClient(){
    return client;
}
