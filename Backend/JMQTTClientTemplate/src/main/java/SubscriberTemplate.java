import org.eclipse.paho.client.mqttv3.IMqttClient;
import org.eclipse.paho.client.mqttv3.MqttException;

import java.util.concurrent.Executors;
import java.util.concurrent.ThreadPoolExecutor;

public class SubscriberTemplate {

    private IMqttClient client;

    private ThreadPoolExecutor executor = (ThreadPoolExecutor) Executors.newFixedThreadPool(3);

    public SubscriberTemplate(IMqttClient client) {
        this.client = client;
    }

    public void subscribe(String topicIn) {
        // CountDownLatch receivedSignal = new CountDownLatch(10);
        try {
            client.subscribe(topicIn, (topic, msg) -> {
                executor.submit(new MessageProcessor(msg));
            });
            // receivedSignal.await(1, TimeUnit.MINUTES);
        } catch (MqttException e) {
            e.printStackTrace();
        }
    }

}
