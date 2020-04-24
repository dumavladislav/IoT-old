#include <TinyGPS++.h>
#include <SoftwareSerial.h>

class GpsData {

public:
    bool gpsDataValid = false;
    double lat = 0;
    double lng = 0;
    bool gpsDateValid = false;
    uint8_t gpsDay = 0;
    uint8_t gpsMonth = 0;
    uint8_t gpsYear = 0;
    bool gpsTimeValid = false;
    uint8_t gpsHour = 0;
    uint8_t gpsMinute = 0;
    uint8_t gpsSecond = 0;
    uint8_t gpsCentisecond = 0;

    String toString();
};    

class GPSClient {

public:

    GPSClient();
    void init();
    GpsData readGpsData();

private:

    // The TinyGPS++ object
    TinyGPSPlus gps;
    SoftwareSerial* ss;

};

