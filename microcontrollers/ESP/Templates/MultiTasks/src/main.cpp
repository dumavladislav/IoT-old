#include <Arduino.h>

int PIN_SHOCK_D0 = 26;
int PIN_SHOCK_A0 = 27;

int PIN_WHITE_LED = 13;
int PIN_BLUE_LED = 12;

int WHITE_LED_TIME = 1;
int BLUE_LED_TIME = 4;
float_t QUEUE_POLL_TIME = 0.6;

TaskHandle_t task1Handler = NULL;
TaskHandle_t task2Handler = NULL;
TaskHandle_t task3Handler = NULL;

QueueHandle_t totalQueue;

void task1Code(void *parameter)
{
  uint32_t prevTime = 0;


  for (;;)
  {    
    if((millis() - prevTime) > (WHITE_LED_TIME * 1000 / 2 )) {
      if (digitalRead(PIN_WHITE_LED) == HIGH) {
        digitalWrite(PIN_WHITE_LED, LOW);
        prevTime = millis();
      }
      else
      {
        digitalWrite(PIN_WHITE_LED, HIGH);
        prevTime = millis();
      }

      xQueueGenericSendFromISR(totalQueue, "1", pdFALSE, queueSEND_TO_BACK);
      delay(WHITE_LED_TIME * 1000 / 2 );
    }
  }
}

void task2Code(void *parameter)
{
  uint32_t prevTime = 0;

  for (;;)
  {
    if((millis() - prevTime) > (BLUE_LED_TIME * 1000 / 2 )) {
      if (digitalRead(PIN_BLUE_LED) == HIGH)
        digitalWrite(PIN_BLUE_LED, LOW);
      else
        digitalWrite(PIN_BLUE_LED, HIGH);

      xQueueGenericSendFromISR(totalQueue, "2", pdFALSE, queueSEND_TO_BACK);
      delay(BLUE_LED_TIME * 1000 / 2);
    }
  }
}

void task3Code(void *parameter)
{
  for (;;)
  {
    Serial.print("TASK3: Messaesin queue: ");
    Serial.println(uxQueueMessagesWaiting(totalQueue));
    if (!xQueueIsQueueEmptyFromISR(totalQueue)) {
      char readChar;
      xQueueGenericReceive(totalQueue, &readChar, ( TickType_t ) 10 , false);
      Serial.print("TASK3: read value is ");
      Serial.println(readChar);
    }
    delay(QUEUE_POLL_TIME / 2 * 1000);
  }
}

void setup()
{
  Serial.begin(115200);

  pinMode(PIN_SHOCK_D0, INPUT);
  pinMode(PIN_SHOCK_A0, INPUT);

  pinMode(PIN_WHITE_LED, OUTPUT);
  pinMode(PIN_BLUE_LED, OUTPUT);

  totalQueue = xQueueCreate( 10, sizeof( char ) );

  xTaskCreatePinnedToCore(
      task1Code, "TASK1", 1000 //const uint32_t usStackDepth
      ,
      NULL //void *constpvParameters
      ,
      1 //UBaseType_t uxPriority
      ,
      &task1Handler //TaskHandle_t *constpvCreatedTask
      ,
      1 //const BaseType_t xCoreID
  );

  xTaskCreatePinnedToCore(
      task2Code, "TASK2", 1000 //const uint32_t usStackDepth
      ,
      NULL //void *constpvParameters
      ,
      1 //UBaseType_t uxPriority
      ,
      &task2Handler //TaskHandle_t *constpvCreatedTask
      ,
      0 //const BaseType_t xCoreID
  );

  xTaskCreatePinnedToCore(
      task3Code, "TASK3", 1000 //const uint32_t usStackDepth
      ,
      NULL //void *constpvParameters
      ,
      1 //UBaseType_t uxPriority
      ,
      &task3Handler //TaskHandle_t *constpvCreatedTask
      ,
      1 //const BaseType_t xCoreID
  );
}

void loop()
{
  // digitalWrite(PIN_BLUE_LED, HIGH);

  // Serial.println(digitalRead(PIN_SHOCK_D0));
  // Serial.println(analogRead(PIN_SHOCK_A0));

  // delay(1000);

  vTaskDelete(NULL);
}

// sudo chmod a+rw /dev/ttyUSB0