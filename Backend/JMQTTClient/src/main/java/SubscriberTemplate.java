import org.eclipse.paho.client.mqttv3.IMqttClient;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;

import static java.lang.Thread.sleep;

public class SubscriberTemplate {

    private IMqttClient client;

    public SubscriberTemplate(IMqttClient client) {
        this.client = client;
    }

    public void subscribe(String topicIn) {
        // CountDownLatch receivedSignal = new CountDownLatch(10);
        try {
            client.subscribe(topicIn, (topic, msg) -> {
                Thread messageProcessingTh = new Thread(new Runnable() {
                    private MqttMessage msg;
                    public Runnable init(MqttMessage msg) {
                        this.msg = msg;
                        return this;
                    }

                    @Override
                    public void run() {
                        byte[] payload = this.msg.getPayload();
                        try {
                            sleep(10000); // - just to test multi threading
                        } catch (InterruptedException e) {
                            e.printStackTrace();
                        }
                        System.out.println(new String(payload));
                        // receivedSignal.countDown();
                    }
                }.init(msg));
                messageProcessingTh.start();

            });
            // receivedSignal.await(1, TimeUnit.MINUTES);
        } catch (MqttException e) {
            e.printStackTrace();
        }
    }

}
