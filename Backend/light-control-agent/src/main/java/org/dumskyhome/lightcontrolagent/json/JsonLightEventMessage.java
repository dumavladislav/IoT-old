package org.dumskyhome.lightcontrolagent.json;

public class JsonLightEventMessage extends JsonMqttMessage{
    private JsonLightEventMessageData data;

    public JsonLightEventMessageData getData() {
        return data;
    }

    public void setData(JsonLightEventMessageData data) {
        this.data = data;
    }
}
