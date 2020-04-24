package org.dumskyhome.authorizationservice.Json;

public class JsonAuthorisationRequestMessage extends JsonMqttMessage {

    JsonAuthorisationRequestData data = new JsonAuthorisationRequestData();

    public JsonAuthorisationRequestData getData() {
        return data;
    }

    public void setData(JsonAuthorisationRequestData data) {
        this.data = data;
    }
}
