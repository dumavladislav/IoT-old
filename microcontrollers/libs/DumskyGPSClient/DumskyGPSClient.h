#include <TinyGPS++.h>
#include <SoftwareSerial.h>

class GpsData {

public:
    bool gpsDataValid = false;
    double lat;
    double lng;
    bool gpsDateValid = false;
    uint8_t gpsDay;
    uint8_t gpsMonth;
    uint8_t gpsYear;
    bool gpsTimeValid = false;
    uint8_t gpsHour;
    uint8_t gpsMinute;
    uint8_t gpsSecond;
    uint8_t gpsCentisecond;

    String toString();
};    

class GPSClient {

public:

    GPSClient();
    ~GPSClient();
    void init();
    GpsData readGpsData();

private:

    

    static const int RXPin = 4, TXPin = 3;
    static const uint32_t GPSBaud = 9600;

    // The TinyGPS++ object
    TinyGPSPlus gps;
    SoftwareSerial* ss;

};

