#include "GSMConnect.h"

GSMConnect::GSMConnect(int rx_port, int tx_port) {
    //SerialAT.begin(115200, SWSERIAL_8N1, rx_port, tx_port, false, 256);  
    //TinyGsmAutoBaud(SerialAT, GSM_AUTOBAUD_MIN, GSM_AUTOBAUD_MAX);
    //SerialAT.begin(115200);
    //delay(3000);  
}

void GSMConnect::init(Stream* serialStreamGSM) {
    this->serialStreamGSM = serialStreamGSM;
    modem = new TinyGsm(*(this->serialStreamGSM));
    client = new TinyGsmClient(*modem);
}

int8_t GSMConnect::connect(char* apn, char* gprsUser, char* gprsPass) {
    // forceListen();
    
    // modem = new TinyGsm(SerialAT);
    SerialMon.println("Initializing modem->..");
    modem->restart();
    // modem->init();

    SerialMon.print("Modem Info: ");
    SerialMon.println(modem->getModemInfo());

    // #if TINY_GSM_USE_GPRS
    // // Unlock your SIM card with a PIN if needed
    // if ( GSM_PIN && modem.getSimStatus() != 3 ) {
    //     modem.simUnlock(GSM_PIN);
    // }
    // #endif

    modem->gprsConnect(apn, gprsUser, gprsPass);

    SerialMon.print("Waiting for network...");
    if (!modem->waitForNetwork(1000000L)) {
       SerialMon.println(" fail");
        delay(10000);
        return -1;
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
        // delay(10000);
        return -2;
    }
    SerialMon.println(" success");

    if (modem->isGprsConnected()) {
        SerialMon.println("GPRS connected");
    }

    
    return 1;
}

boolean GSMConnect::keepAlive(char* apn, char* gprsUser, char* gprsPass) {
    if(!modem->isGprsConnected()) {
        if(!modem->isNetworkConnected()) connect(apn, gprsUser, gprsPass);
        else
            modem->gprsConnect(apn, gprsUser, gprsPass);
        delay(10000);
    }
    if(!client) client = new TinyGsmClient(*modem);
    return modem->isGprsConnected();
}

TinyGsmClient* GSMConnect::getClient(){
    return client;
}

String GSMConnect::getIMEI() {
    return modem->getIMEI();
}

// void GSMConnect::forceListen() {
//     SerialAT.listen();
// }

boolean GSMConnect::GPRSConnected() {
    return modem->isGprsConnected();
}


boolean GSMConnect::GSMConnected() {
    return modem->isNetworkConnected();
}