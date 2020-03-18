import org.eclipse.paho.client.mqttv3.IMqttClient;
import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.eclipse.paho.client.mqttv3.MqttException;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.Properties;
import java.util.UUID;

public class JMQTTClient {

    public static Properties readProperties() throws FileNotFoundException, IOException {
        File file = new File("./src/main/resources/MqttProperties.conf");
        Properties properties = new Properties();
        try {
            properties.load(new FileReader(file));
            return properties;
        }
        catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
        return null;
    }

    public static void main(String[] args) {
        System.out.println("Hey there");

        //JMQTTClient client = new JMQTTClient();
        Properties mqttProps = null;
        try {
            mqttProps = readProperties();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }

        MqttConnectOptions options = new MqttConnectOptions();
        options.setAutomaticReconnect(true);
        options.setCleanSession(true);
        options.setConnectionTimeout(20);
        options.setUserName(mqttProps.getProperty("mqtt.user"));
        options.setPassword(mqttProps.getProperty("mqtt.password").toCharArray());

        String clientId = UUID.randomUUID().toString();
        try {
            IMqttClient mqttClient = new MqttClient(mqttProps.getProperty("mqtt.serverUrl").toString(),clientId);
            mqttClient.connect(options);

            // Publish example
            PublisherTemplate lcc = new PublisherTemplate(mqttClient);
            lcc.call();

            // Subscribe example
            SubscriberTemplate st = new SubscriberTemplate(mqttClient);
            st.subscribe("test/");

        } catch (MqttException e) {
            e.printStackTrace();
        } catch (Exception e) {
            e.printStackTrace();
        }


    }

}
