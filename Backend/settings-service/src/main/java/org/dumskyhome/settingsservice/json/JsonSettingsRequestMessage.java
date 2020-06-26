package org.dumskyhome.settingsservice.json;

public class JsonSettingsRequestMessage extends JsonMqttMessage {

    JsonSettingsRequestData data;

    public JsonSettingsRequestData getData() {
        return data;
    }

    public void setData(JsonSettingsRequestData data) {
        this.data = data;
    }
}
