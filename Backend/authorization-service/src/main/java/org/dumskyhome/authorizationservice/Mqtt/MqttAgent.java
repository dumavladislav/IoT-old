package org.dumskyhome.authorizationservice.Mqtt;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.DeserializationFeature;
import com.fasterxml.jackson.databind.ObjectMapper;
import org.dumskyhome.authorizationservice.Json.*;
import org.dumskyhome.authorizationservice.service.AuthorizationService;
import org.eclipse.paho.client.mqttv3.*;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.context.annotation.PropertySource;
import org.springframework.core.env.Environment;
import org.springframework.http.ResponseEntity;
import org.springframework.stereotype.Component;

import java.util.concurrent.CompletableFuture;

@Component
@PropertySource("classpath:mqtt.properties")
public class MqttAgent implements MqttCallback {

    private static final Logger logger = LoggerFactory.getLogger(MqttAgent.class);

    private MqttClient mqttClient;

    @Value("${mqtt.clientId}")
    private String mqttClientId;
    //private ThreadPoolExecutor executor;
    //@Autowired
    //MqttMessageProcessor mqttMessageProcessor;

    @Autowired
    private AuthorizationService authorizationService;

    @Autowired
    Environment env;

    private ObjectMapper objectMapper;

    MqttAgent() {
        objectMapper = new ObjectMapper();
        objectMapper.disable(DeserializationFeature.FAIL_ON_UNKNOWN_PROPERTIES);
        objectMapper.enable(DeserializationFeature.ACCEPT_EMPTY_STRING_AS_NULL_OBJECT);
        objectMapper.disable(DeserializationFeature.FAIL_ON_MISSING_CREATOR_PROPERTIES);
        objectMapper.disable(DeserializationFeature.FAIL_ON_IGNORED_PROPERTIES);
    }

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

    private boolean subscribeToTopics() {
        try {
            mqttClient.subscribe(env.getProperty("mqtt.topic.authorizationRequests"));
            mqttClient.subscribe(env.getProperty("mqtt.topic.registrationResponses"));
            mqttClient.setCallback(this);
            return true;
        } catch (MqttException e) {
            e.printStackTrace();
        }
        return false;
    }

    public boolean runMqttService() {
        init();
        if(connect()) {
            logger.info("Connected to MQTT");
            return subscribeToTopics();
        }
        return false;
    }

    public void sendMessage(String topic, String messageString, int QoS) {
        MqttMessage mqttMessage = new MqttMessage();
        mqttMessage.setQos(QoS);
        mqttMessage.setPayload(messageString.getBytes());
        try {
            mqttClient.publish(topic, mqttMessage);
        } catch (MqttException e) {
            e.printStackTrace();
        }
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
        // ObjectMapper objectMapper = new ObjectMapper();

        logger.info("TOPIC:"+ s + " || MESSAGE RECEIVED: " + mqttMessage.toString());
        if (s.equals(env.getProperty("mqtt.topic.authorizationRequests"))) {
            logger.info("Authorization request received. Parsing the request....");
            JsonAuthorisationRequestMessage mqttMessageJson = objectMapper.readValue(mqttMessage.toString(), JsonAuthorisationRequestMessage.class);

            logger.info(mqttMessageJson.getData().getRequestType());
            // CompletableFuture<Integer> authorized;
            authorizationService.checkAuthorization(mqttMessageJson.getHeader()).<Integer>thenApply(messageHeader -> {
                if (messageHeader.getAuthorized() == 1) {
                    finalizeAuthorization(messageHeader, 1);
                }
                else {
                    sendRegistrationRequest(messageHeader);
                }
                return null;
            });
         }

        if (s.equals(env.getProperty("mqtt.topic.registrationResponses"))) {
            JsonRegistrationResponseMessage message = objectMapper.readValue(mqttMessage.toString(), JsonRegistrationResponseMessage.class);
            authorizationService.finalizeRegistration(message);
            finalizeAuthorization(message.getHeader(), message.getData().getDecision());
        }
    }

    public void finalizeAuthorization(JsonMqttMessageHeader messageHeader, Integer decision) {
        ObjectMapper objectMapper = new ObjectMapper();
        JsonAuthorizationConfirmationMessage jsonAuthorizationConfirmationMessage = new JsonAuthorizationConfirmationMessage();
        jsonAuthorizationConfirmationMessage.setHeader(messageHeader);
        jsonAuthorizationConfirmationMessage.setData(new JsonAuthorizationConfirmationData(decision == 1 ? true : false, messageHeader.getToken()));

        try {
            String message = objectMapper.writeValueAsString(jsonAuthorizationConfirmationMessage);
            logger.info(message);
            /*mqttAgent.*/sendMessage(env.getProperty("mqtt.topic.authorizationResponses"), message, 2);
        } catch (JsonProcessingException e) {
            e.printStackTrace();
        }

    }

    public void sendRegistrationRequest(JsonMqttMessageHeader messageHeader) {
        ObjectMapper objectMapper = new ObjectMapper();
        JsonRegistrationRequestMessage jsonRegistrationRequestMessage = new JsonRegistrationRequestMessage();
        jsonRegistrationRequestMessage.setHeader(messageHeader);
        jsonRegistrationRequestMessage.setData(new JsonRegistrationRequestData("registration"));
        try {
            String message = objectMapper.writeValueAsString(jsonRegistrationRequestMessage);
            logger.info(message);
            sendMessage(env.getProperty("mqtt.topic.registrationRequests"), message, 2);
        } catch (JsonProcessingException e) {
            e.printStackTrace();
        }
    }

    @Override
    public void deliveryComplete(IMqttDeliveryToken iMqttDeliveryToken) {
        logger.info("DELIVERY COMPLETE");
    }
}
