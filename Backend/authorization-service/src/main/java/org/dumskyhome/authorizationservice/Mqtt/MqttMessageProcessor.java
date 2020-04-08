package org.dumskyhome.authorizationservice.Mqtt;

import org.dumskyhome.authorizationservice.persistence.DAO.AuthorizationServiceDao;
import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.MqttCallback;
import org.eclipse.paho.client.mqttv3.MqttMessage;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.annotation.PropertySource;
import org.springframework.core.env.Environment;
import org.springframework.http.ResponseEntity;
import org.springframework.scheduling.annotation.Async;
import org.springframework.scheduling.annotation.EnableAsync;
import org.springframework.stereotype.Component;

import java.util.concurrent.CompletableFuture;

import static java.lang.Thread.sleep;

@Component
@PropertySource("classpath:mqtt.properties")
public class MqttMessageProcessor implements MqttCallback {

    private static final Logger logger = LoggerFactory.getLogger(MqttMessageProcessor.class);

    @Autowired
    private Environment env;

    @Autowired
    private AuthorizationServiceDao authorizationServiceDao;

    @Override
    public void connectionLost(Throwable throwable) {
        logger.error("CONNECTION LOST");
    }

    @Override
    public void messageArrived(String s, MqttMessage mqttMessage) throws Exception {
        logger.info("TOPIC:"+ s + " || MESSAGE RECEIVED: " + mqttMessage.toString());
        if (s.equals(env.getProperty("mqtt.topic.authorizationRequests"))) {
            authorizationServiceDao.checkAuthorization(mqttMessage.toString()).<ResponseEntity>thenApply(ResponseEntity::ok);
        }
    }

    @Override
    public void deliveryComplete(IMqttDeliveryToken iMqttDeliveryToken) {
        logger.info("DELIVERY COMPLETE");
    }

}
