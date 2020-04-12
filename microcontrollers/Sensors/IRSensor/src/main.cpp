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

    if (mySwitch.getReceivedValue() == 16736113)
    {
      Serial.write("LOCK Button pressed");
      digitalWrite(LED, HIGH);
    }
    if (mySwitch.getReceivedValue() == 11169970)
    {
      Serial.write("UNLOCK Button pressed");
      digitalWrite(LED, LOW);
    }
    if (mySwitch.getReceivedValue() == 16736114)
    {
      Serial.write("HORN Button pressed");
      digitalWrite(LED, LOW);
    }
    if (mySwitch.getReceivedValue() == 16736120)
    {
      Serial.write("OPEN TANK Button pressed");
      digitalWrite(LED, LOW);
    }
  

    mySwitch.resetAvailable();
  }
}