#include "MQTTLightControl.h"

MQTTLightControl::MQTTLightControl(char* devId, char* mqttServer, int mqttPort/*, MQTT_CALLBACK_SIGNATURE*/) {
    deviceId = devId;

    WiFiClient espClient;
    client = PubSubClient(espClient);
    client.setServer(mqttServer, mqttPort);
     
    
    MSState = LOW;
}

void MQTTLightControl::setCallback(MQTT_CALLBACK_SIGNATURE) {
  client.setCallback(callback);
}

void MQTTLightControl::wifiConnect(char* ssid, char* password) {
    delay(10);
    // We start by connecting to a WiFi network
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    randomSeed(micros());

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void MQTTLightControl::mqttConnect(char* mqttUsr, char* mqttPasswd) {
    // Loop until we're reconnected
    while (!client.connected()) {
        deviceSessionId = (char*)((String)deviceId + String(random(0xffff), HEX)).c_str();
        //Serial.print("Attempting MQTT connection...");
        // Attempt to connect
        if (client.connect(deviceId, mqttUsr, mqttPasswd)) {
        //Serial.println("connected");
        // Once connected, publish an announcement...
        client.publish(GREETING_TPC, ((String)deviceId + " connected").c_str());
        // ... and resubscribe
        client.subscribe(DEVICE_OPERATION_CONTROL_TPC);
        } else {
        //Serial.print("failed, rc=");
        //Serial.print(client.state());
        //Serial.println(" try again in 5 seconds");
        // Wait 5 seconds before retrying
        delay(5000);
        }
    }
};

void MQTTLightControl::sendMessage(char* topicName, char message[]) {
    char msg[50];
    // current date/time based on current system
    time_t now = time(0);
    // convert now to string form
    char* dt = ctime(&now);
    snprintf (msg, 50, "%s%s: %s", dt, deviceSessionId, message);
    client.publish(topicName, msg);
}

void MQTTLightControl::keepAlive(char* mqttUsr, char* mqttPasswd) {
    if (!client.connected()) {
        mqttConnect(mqttUsr, mqttPasswd);
    }
    client.loop();
}

void MQTTLightControl::updateState(int newState) {
  switch (getOperationMode())
  {
  case operationModes::MSDRIVEN:
    msDrivenOperation(newState);
    break;
  case operationModes::ON:
    onOperation();
    break;
  case operationModes::OFF:
    offOperation();
    break;
  default:
    break;
  }
}


operationModes MQTTLightControl::getOperationMode() {
    return operationMode;
}

void MQTTLightControl::setOperationMode(int mode) {
    Serial.print("DEVICE ");
    Serial.print(deviceId);
    Serial.print(": Changing operation mode to ");
    Serial.println(mode);
    operationMode = decodeOperationMode(mode);
    sendOperationMode();
}

void MQTTLightControl::sendMSState(boolean currState) {

};

operationModes MQTTLightControl::decodeOperationMode(int intVar) {
    operationModes enumVar = static_cast<operationModes>(intVar);
}

int MQTTLightControl::getState() {
  return MSState;
}

void MQTTLightControl::sendMSState() {
  char message[50];
  snprintf (message, 50, "%d", MSState);
  sendMessage(MOTION_SENSOR_STATE_TPC, message);
}


void MQTTLightControl::sendOperationMode() {
  char message[50];
  snprintf (message, 50, "%d", operationMode);
  sendMessage(DEVICE_OPERATION_MODE_TPC, message);
}


void MQTTLightControl::msDrivenOperation(int newState) {
	int newStateDigital = 0;
	if (newState >= 333) newStateDigital = HIGH;
	else newStateDigital = LOW;
	
	if (MSState != newStateDigital) {
		MSState = newStateDigital;
		sendMSState();
	}
}

void MQTTLightControl::offOperation() {
	if(MSState == HIGH) {
	  MSState = LOW;
	  sendMSState(MSState);
	}
}

void MQTTLightControl::onOperation() {
	if(MSState == LOW) {
	  startTimeOnOperationMode = millis();
	  MSState = HIGH;
	  sendMSState(MSState);
	}
	else
	{
	  float currTime = millis();
	  if((currTime - startTimeOnOperationMode) >= MAX_ON_OPERATION_MODE_DURATION_MS) offOperation();
	}
}









