package org.dumskyhome.authorizationservice.Json;

public class JsonRegistrationRequestMessage extends JsonMqttMessage {
    JsonRegistrationRequestData data = new JsonRegistrationRequestData();

    public JsonRegistrationRequestData getData() {
        return data;
    }

    public void setData(JsonRegistrationRequestData data) {
        this.data = data;
    }
}
