import org.eclipse.paho.client.mqttv3.MqttMessage;

import static java.lang.Thread.sleep;

public class MessageProcessor implements Runnable {

    private MqttMessage msg;

    public MessageProcessor(MqttMessage msg) {
        this.msg = msg;
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
}
