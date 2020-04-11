#include <Arduino.h>

#include <SoftwareSerial.h>
SoftwareSerial SIM800(8, 9);        // 8 - RX Arduino (TX SIM800L), 9 - TX Arduino (RX SIM800L)
void setup() {
  Serial.begin(9600);               // Скорость обмена данными с компьютером
  Serial.println("Start!");
  SIM800.begin(9600);               // Скорость обмена данными с модемом
  SIM800.println("AT");
  SIM800.println("AT+CMGF=1");
  SIM800.println("AT+CMGS=\"+79653650849\"");
}

void loop() {
  if (SIM800.available())           // Ожидаем прихода данных (ответа) от модема...
    Serial.write(SIM800.read());    // ...и выводим их в Serial
  if (Serial.available())           // Ожидаем команды по Serial...
    SIM800.write(Serial.read());    // ...и отправляем полученную команду модему
}