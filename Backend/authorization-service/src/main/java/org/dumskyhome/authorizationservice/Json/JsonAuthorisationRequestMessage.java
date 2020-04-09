package org.dumskyhome.authorizationservice.Json;

public class JsonAuthorisationRequestMessage extends JsonMqttMessage {

    JsonAuthorisationRequestData data;

    public JsonAuthorisationRequestData getData() {
        return data;
    }

    public void setData(JsonAuthorisationRequestData data) {
        this.data = data;
    }
}
