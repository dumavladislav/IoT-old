#include <Arduino.h>
#include <Wire.h>
// #include <RtcDS3231.h>
#include <RTClib.h>



#define countof(a) (sizeof(a) / sizeof(a[0]))

class RealTimeClock {

public:

    RealTimeClock();
    void init();
    String getDateTime();
    String getDate();
    String getTime();

private:

    // RtcDS3231<TwoWire> Rtc{Wire};
    DateTime readDateTime();

    RTC_DS1307 rtc;

};