import org.eclipse.paho.client.mqttv3.IMqttClient;
import org.eclipse.paho.client.mqttv3.MqttMessage;

import java.util.Random;
import java.util.concurrent.Callable;

public class LightControlCollector implements Callable<Void> {

    private IMqttClient client;

    public LightControlCollector(IMqttClient client) {
        this.client = client;
    }

    @Override
    public Void call() throws Exception {
        if(!client.isConnected()) return null;

        MqttMessage msg = settingsMessage();
        msg.setQos(0);
        msg.setRetained(true);
        client.publish("/DEVICES/settings/mostionSensors", msg);
        return null;
    }

    private MqttMessage settingsMessage() {
        Random rnd = new Random();
        double temp =  80 + rnd.nextDouble() * 20.0;
        byte[] payload = String.format("T:%04.2f",temp)
                .getBytes();
        return new MqttMessage(payload);
    }

}
