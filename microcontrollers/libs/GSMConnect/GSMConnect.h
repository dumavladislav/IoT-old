#define TINY_GSM_MODEM_SIM800
#define TINY_GSM_USE_GPRS true
#define DUMP_AT_COMMANDS
#define SerialMon Serial
#define TINY_GSM_DEBUG SerialMon

#define GSM_AUTOBAUD_MIN 9600
#define GSM_AUTOBAUD_MAX 115200

// Just in case someone defined the wrong thing..
#if TINY_GSM_USE_GPRS && not defined TINY_GSM_MODEM_HAS_GPRS
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
#endif

#include <SoftwareSerial.h>
#include <TinyGsmClient.h>

namespace Dumsky {

class GSMConnect {

public:
    GSMConnect();
    void init();
    TinyGsmClient* getClient();

private:

    

    // Your GPRS credentials, if any
    const char apn[] = "YourAPN";
    const char gprsUser[] = "";
    const char gprsPass[] = "";

    TinyGsm* modem;
    TinyGsmClient* client;

};

}