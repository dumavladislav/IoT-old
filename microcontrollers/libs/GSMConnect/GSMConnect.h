#define TINY_GSM_MODEM_SIM800

#if !defined(TINY_GSM_RX_BUFFER)
#define TINY_GSM_RX_BUFFER 1024
#endif

#define TINY_GSM_USE_GPRS true
// #define DUMP_AT_COMMANDS
#define SerialMon Serial

// #define TINY_GSM_DEBUG SerialMon

#define GSM_AUTOBAUD_MIN 9600
#define GSM_AUTOBAUD_MAX 115200

// Just in case someone defined the wrong thing..
/*#if TINY_GSM_USE_GPRS && not defined TINY_GSM_MODEM_HAS_GPRS
#undef TINY_GSM_USE_GPRS
#undef TINY_GSM_USE_WIFI
#define TINY_GSM_USE_GPRS false
#define TINY_GSM_USE_WIFI true
#endif
#if TINY_GSM_USE_WIFI && not defined TINY_GSM_MODEM_HAS_WIFI
#undef TINY_GSM_USE_GPRS
#undef TINY_GSM_USE_WIFI
#define TINY_GSM_USE_GPRS true
#define TINY_GSM_USE_WIFI false
#endif*/

#include <SoftwareSerial.h>
#include <TinyGsmClient.h>

//namespace Dumsky {

class GSMConnect {

public:
    GSMConnect(int rx_port, int tx_port);
    void init(Stream* serialStreamGSM);
    int8_t connect(char* apn, char* gprsUser, char* gprsPass);
    boolean keepAlive(char* apn, char* gprsUser, char* gprsPass);
    boolean GSMConnected();
    boolean GPRSConnected();
    void forceListen();
    TinyGsmClient* getClient();
    String getIMEI();
    // IPAddress getIP();

private:
//    int8_t rx;
//    int8_t tx; 
    // SoftwareSerial SerialAT = SoftwareSerial(8, 9); // RX, TX
    // SoftwareSerial SerialAT{5, 18}; // RX, TX
    Stream* serialStreamGSM;

    TinyGsm* modem;
    TinyGsmClient* client;

};

//}