#include <Arduino.h>
#include <U8g2lib.h>

class DumskyOLED {

public:

    void init();
    void clear();
    void addString(String str);
    void renderString(String str);
    void show();

private:

    uint8_t currString = 0;
    uint8_t width, height = 0;

    U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C u8g2{U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE};
    

};