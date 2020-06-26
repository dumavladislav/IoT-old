#include <DumskyOLED.h>


void DumskyOLED::init() {
    u8g2.begin();
    u8g2.clearBuffer();					// clear the internal memory
    //u8g2.setFont(u8g2_font_ncenB08_tr);	// choose a suitable font
    u8g2.setFont(u8g2_font_crox5tb_tf);
}


void DumskyOLED::clear() {
    u8g2.clearBuffer();					// clear the internal memory
    // u8g2.setFont(u8g2_font_ncenB08_tr);	// choose a suitable font
    u8g2.setFont(u8g2_font_crox5tb_tf);
    currString = 0;
}

void DumskyOLED::renderString(String str) {
    addString(str);
    show(); 
}

void DumskyOLED::addString(String str) {
    currString++;
    if (currString > 3) {
        clear();
        currString = 0;
    } 
    // u8g2.drawStr(0, currString * 10, str.c_str()); 
    u8g2.drawStr(0, currString * 25, str.c_str()); 
}

void DumskyOLED::show() {
    u8g2.sendBuffer();
}



