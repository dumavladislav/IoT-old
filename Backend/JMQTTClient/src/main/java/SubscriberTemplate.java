import org.eclipse.paho.client.mqttv3.IMqttClient;
import org.eclipse.paho.client.mqttv3.MqttException;

public class SubscriberTemplate {

    private IMqttClient client;

    public SubscriberTemplate(IMqttClient client) {
        this.client = client;
    }

    public void subscribe(String topicIn) {
        // CountDownLatch receivedSignal = new CountDownLatch(10);
        try {
            client.subscribe(topicIn, (topic, msg) -> {
                byte[] payload = msg.getPayload();
                // sleep(10000); // - just to test multi threading
                System.out.println(new String(payload));
                // receivedSignal.countDown();
            });
            // receivedSignal.await(1, TimeUnit.MINUTES);
        } catch (MqttException e) {
            e.printStackTrace();
        }
    }

}
