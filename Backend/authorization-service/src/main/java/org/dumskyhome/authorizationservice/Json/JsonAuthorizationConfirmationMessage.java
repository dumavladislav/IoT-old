package org.dumskyhome.authorizationservice.Json;

import com.fasterxml.jackson.annotation.JsonInclude;

public class JsonAuthorizationConfirmationMessage extends JsonMqttMessage {
    JsonAuthorizationConfirmationData data;

    public JsonAuthorizationConfirmationData getData() {
        return data;
    }

    public void setData(JsonAuthorizationConfirmationData data) {
        this.data = data;
    }
}
