package org.dumskyhome.lightcontrolagent.MQTT;

import com.fasterxml.jackson.databind.DeserializationFeature;
import com.fasterxml.jackson.databind.ObjectMapper;
import org.dumskyhome.lightcontrolagent.persistence.DAO.HomeAutomationDAO;
import org.eclipse.paho.client.mqttv3.*;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.context.annotation.PropertySource;
import org.springframework.core.env.Environment;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.stereotype.Component;

import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStream;
import java.util.Properties;
import java.util.UUID;
import java.util.concurrent.Executors;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.function.Function;

@Component
@PropertySource("classpath:mqtt.properties")
public class MqttAgent implements MqttCallback {

    private static final Logger logger = LoggerFactory.getLogger(MqttAgent.class);

//    private Properties properties;
    private MqttClient mqttClient;

    @Value("${mqtt.clientId}")
    private String mqttClientId;

    ThreadPoolExecutor executor;

    private ObjectMapper objectMapper;

    @Autowired
    Environment env;


//    @Autowired
//    private MqttMessageProcessor mqttMessageProcessor;

    @Autowired
    private HomeAutomationDAO homeAutomationDAO;

    MqttAgent() {
        objectMapper = new ObjectMapper();
        objectMapper.disable(DeserializationFeature.FAIL_ON_UNKNOWN_PROPERTIES);
        objectMapper.enable(DeserializationFeature.ACCEPT_EMPTY_STRING_AS_NULL_OBJECT);
        objectMapper.disable(DeserializationFeature.FAIL_ON_MISSING_CREATOR_PROPERTIES);
        objectMapper.disable(DeserializationFeature.FAIL_ON_IGNORED_PROPERTIES);
    }

/*    public MqttAgent() {
        properties = new Properties();
        try {
            InputStream input = null;
            input = ClassLoader.getSystemClassLoader().getResourceAsStream("mqtt.properties");
            properties.load(input);
            //properties.load(new FileReader(new File("src/main/resources/mqtt.properties")));
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
    }*/

    private void init() {
        try {
            mqttClient = new MqttClient(env.getProperty("mqtt.serverUrl"), mqttClientId);
        } catch (MqttException e) {
            e.printStackTrace();
        }
    }


    private boolean connect() {

        try {
            MqttConnectOptions mqttConnectOptions = new MqttConnectOptions();
            mqttConnectOptions.setUserName(env.getProperty("mqtt.user"));
            mqttConnectOptions.setPassword(env.getProperty("mqtt.password").toCharArray());
            mqttConnectOptions.setConnectionTimeout(Integer.parseInt(env.getProperty("mqtt.connectionTimeout")));
            mqttConnectOptions.setAutomaticReconnect(true);
            mqttConnectOptions.setCleanSession(true);

            mqttClient.connect(mqttConnectOptions);
            //if (mqttClient.isConnected()) executor = (ThreadPoolExecutor) Executors.newFixedThreadPool(3);
            return mqttClient.isConnected();

        } catch (MqttException e) {
            e.printStackTrace();
        }
        return false;
    }

    public boolean isConnected() {
        return mqttClient.isConnected();
    }

    private boolean subscribeToTopics() {
        try {
            mqttClient.subscribe(env.getProperty("mqtt.topic.haEvents"));
            mqttClient.setCallback(this);
            return true;
        } catch (MqttException e) {
            e.printStackTrace();
        }
        return false;
    }

    /*public void subscribeToHaEvents(String subscribeTopic) {
        if (isConnected()) {
            try {
                if (subscribeTopic == null) subscribeTopic = properties.getProperty("mqtt.eventsTopic");
                mqttClient.subscribe(subscribeTopic);
                mqttClient.setCallback(this);
            } catch (MqttException e) {
                System.out.println("Error subscribe to topic" + subscribeTopic);
                e.printStackTrace();
            }
        }
    }*/

    private static Function<Throwable, ResponseEntity<? extends Long>> handleGetCarFailure = throwable -> {
        logger.error("Failed to read records: {}", throwable);
        return ResponseEntity.status(HttpStatus.INTERNAL_SERVER_ERROR).build();
    };

    /*private MqttCallback saveMqttMessage (topic, msg) -> {
        // executor.submit(new MqttMessageProcessor(msg, eventsService));
        eventsService.saveMotionEvent(0).<ResponseEntity>thenApply(ResponseEntity::ok)
                .exceptionally(handleGetCarFailure);
    }*/

    public boolean runMqttService() {
        init();
        if(connect()) {
            logger.info("Connected to MQTT");
            return subscribeToTopics();
        }
        return false;
    }

    @Override
    public void connectionLost(Throwable throwable) {
        logger.error("CONNECTION LOST");
        while(!connect()) {
            logger.info("RECONNECTION ATTEMPT");
            try {
                Thread.sleep(5000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        };
        logger.error("CONNECTION RESTORED");
        subscribeToTopics();
    }

    @Override
    public void messageArrived(String s, MqttMessage mqttMessage) throws Exception {
        logger.info("MESSAGE ARRIVED!");
        byte[] payload = mqttMessage.getPayload();
        String payloadStr = new String(payload);
        int newMotionState = Integer.parseInt(payloadStr.substring(payloadStr.length()-1));     //(int) (payload[payload.length-1] & 0xFF); //intBuf.get();
        homeAutomationDAO.saveMotionEvent(newMotionState).<ResponseEntity>thenApply(ResponseEntity::ok)
        //.exceptionally(handleGetCarFailure)
        ;
    }

    @Override
    public void deliveryComplete(IMqttDeliveryToken iMqttDeliveryToken) {

    }
}
