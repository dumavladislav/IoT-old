package org.dumskyhome.settingsservice.json;

public class JsonSettingsResponseMessage extends JsonMqttMessage {

    JsonSettingsResponseData data;

    public JsonSettingsResponseData getData() {
        return data;
    }

    public void setData(JsonSettingsResponseData data) {
        this.data = data;
    }
}
