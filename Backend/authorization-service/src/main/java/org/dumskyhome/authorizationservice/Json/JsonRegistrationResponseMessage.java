package org.dumskyhome.authorizationservice.Json;

public class JsonRegistrationResponseMessage extends JsonMqttMessage {
    JsonRegistrationResponseData data;

    public JsonRegistrationResponseData getData() {
        return data;
    }

    public void setData(JsonRegistrationResponseData data) {
        this.data = data;
    }
}
