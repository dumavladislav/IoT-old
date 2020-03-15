#include <RCSwitch.h>
RCSwitch mySwitch = RCSwitch();
int LED = 11;

void setup()
{
  Serial.begin(9600);
  mySwitch.enableReceive(0); // Receiver on interrupt 0 => that is pin #2
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
}

void loop()
{

  if (mySwitch.available())
  {

    int value = mySwitch.getReceivedValue();
    Serial.println(value);
    if (value == 0)
    {
      Serial.print("Unknown encoding");
    }
    else
    {

      Serial.print("Received ");
      Serial.print(mySwitch.getReceivedValue());
      Serial.print(" / ");
      Serial.print(mySwitch.getReceivedBitlength());
      Serial.print("bit ");
      Serial.print("Protocol: ");
      Serial.println(mySwitch.getReceivedProtocol());
    }

    if (mySwitch.getReceivedValue() == 4152588)
    {
      digitalWrite(LED, HIGH);
    }
    if (mySwitch.getReceivedValue() == 4152624)
    {
      digitalWrite(LED, LOW);
    }
    if (mySwitch.getReceivedValue() == 15954498)
    {
      digitalWrite(LED, LOW);
      delay(1000);
      digitalWrite(LED, HIGH);
    }

    mySwitch.resetAvailable();
  }
}