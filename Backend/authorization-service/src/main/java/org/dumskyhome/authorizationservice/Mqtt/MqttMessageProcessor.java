package org.dumskyhome.authorizationservice.Mqtt;

import com.fasterxml.jackson.databind.DeserializationFeature;
import com.fasterxml.jackson.databind.ObjectMapper;
import org.dumskyhome.authorizationservice.Json.JsonAuthorisationRequestMessage;
import org.dumskyhome.authorizationservice.Json.JsonMqttMessage;
import org.dumskyhome.authorizationservice.Json.JsonMqttMessageHeader;
import org.dumskyhome.authorizationservice.service.AuthorizationService;
import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.MqttCallback;
import org.eclipse.paho.client.mqttv3.MqttMessage;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.annotation.PropertySource;
import org.springframework.core.env.Environment;
import org.springframework.http.ResponseEntity;
import org.springframework.stereotype.Component;

import java.io.FileInputStream;
import java.util.Map;

import static java.lang.Thread.sleep;

@Component
@PropertySource("classpath:mqtt.properties")
public class MqttMessageProcessor implements MqttCallback {

    private static final Logger logger = LoggerFactory.getLogger(MqttMessageProcessor.class);
    private ObjectMapper objectMapper;

    public MqttMessageProcessor() {
        objectMapper = new ObjectMapper();
        objectMapper.disable(DeserializationFeature.FAIL_ON_UNKNOWN_PROPERTIES);
        objectMapper.enable(DeserializationFeature.ACCEPT_EMPTY_STRING_AS_NULL_OBJECT);
    }

    @Autowired
    private Environment env;

    @Autowired
    private AuthorizationService authorizationService;

    @Override
    public void connectionLost(Throwable throwable) {
        logger.error("CONNECTION LOST");
    }

    @Override
    public void messageArrived(String s, MqttMessage mqttMessage) throws Exception {
        // ObjectMapper objectMapper = new ObjectMapper();

        logger.info("TOPIC:"+ s + " || MESSAGE RECEIVED: " + mqttMessage.toString());
        if (s.equals(env.getProperty("mqtt.topic.authorizationRequests"))) {
            JsonAuthorisationRequestMessage mqttMessageJson = objectMapper.readValue(mqttMessage.toString(), JsonAuthorisationRequestMessage.class);

            logger.info(mqttMessageJson.getData().getRequestType());
            authorizationService.checkAuthorization(mqttMessageJson.getHeader().getMacAddress()).<ResponseEntity>thenApply(ResponseEntity::ok);

        }
    }

    @Override
    public void deliveryComplete(IMqttDeliveryToken iMqttDeliveryToken) {
        logger.info("DELIVERY COMPLETE");
    }

}
