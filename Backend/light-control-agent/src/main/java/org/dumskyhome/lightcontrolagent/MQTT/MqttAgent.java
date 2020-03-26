package org.dumskyhome.lightcontrolagent.MQTT;

import org.dumskyhome.lightcontrolagent.persistence.services.EventsService;
import org.eclipse.paho.client.mqttv3.IMqttClient;
import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.stereotype.Component;

import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.util.Properties;
import java.util.UUID;
import java.util.concurrent.Executors;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.function.Function;

@Component
public class MqttAgent {

    private static final Logger LOGGER = LoggerFactory.getLogger(MqttAgent.class);

    private Properties properties;
    IMqttClient mqttClient;
    String clientId;
    ThreadPoolExecutor executor;

    @Autowired
    private MqttMessageProcessor mqttMessageProcessor;

    public MqttAgent() {
        properties = new Properties();
        try {
            properties.load(new FileReader(new File("src/main/resources/MQTT.config")));
            clientId = UUID.randomUUID().toString();
            mqttClient = new MqttClient(properties.getProperty("mqtt.serverUrl"), clientId);
            //this.eventsService = eventsService;
        } catch (IOException e) {
            System.out.println("Error reading MQTT configuration file!");
            e.printStackTrace();
        } catch (MqttException e) {
            System.out.println("MQTT client Exception!");
            e.printStackTrace();
        }
    }

    public boolean connect() {
        MqttConnectOptions mqttConnectOptions = new MqttConnectOptions();
        mqttConnectOptions.setUserName(properties.getProperty("mqtt.user"));
        mqttConnectOptions.setPassword(properties.getProperty("mqtt.password").toCharArray());
        mqttConnectOptions.setAutomaticReconnect(true);
        mqttConnectOptions.setConnectionTimeout(10);
        try {
            mqttClient.connect(mqttConnectOptions);
            if (mqttClient.isConnected()) executor = (ThreadPoolExecutor) Executors.newFixedThreadPool(3);
            return mqttClient.isConnected();
        } catch (MqttException e) {
            System.out.println("MQTT Connection Error!");
            e.printStackTrace();
        }
        return false;
    }

    public boolean isConnected() {
        return mqttClient.isConnected();
    }

    public void subscribeToHaEvents(String subscribeTopic) {
        if (isConnected()) {
            try {
                if (subscribeTopic == null) subscribeTopic = properties.getProperty("mqtt.eventsTopic");
                mqttClient.subscribe(subscribeTopic);
                mqttClient.setCallback(mqttMessageProcessor);
            } catch (MqttException e) {
                System.out.println("Error subscribe to topic" + subscribeTopic);
                e.printStackTrace();
            }
        }
    }

    private static Function<Throwable, ResponseEntity<? extends Long>> handleGetCarFailure = throwable -> {
        LOGGER.error("Failed to read records: {}", throwable);
        return ResponseEntity.status(HttpStatus.INTERNAL_SERVER_ERROR).build();
    };

    /*private MqttCallback saveMqttMessage (topic, msg) -> {
        // executor.submit(new MqttMessageProcessor(msg, eventsService));
        eventsService.saveMotionEvent(0).<ResponseEntity>thenApply(ResponseEntity::ok)
                .exceptionally(handleGetCarFailure);
    }*/


}
