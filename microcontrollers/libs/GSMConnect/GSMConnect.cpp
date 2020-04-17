#include "GSMConnect.h"

Dumsky::GSMConnect::GSMConnect() {
    SerialAT.begin(9600);
}

void Dumsky::GSMConnect::init(char* apn, char* gprsUser, char* gprsPass) {

    this->apn = apn;
    this->gprsUser = gprsUser;
    this->gprsPass = gprsPass;

    modem = new TinyGsm(SerialAT);
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
