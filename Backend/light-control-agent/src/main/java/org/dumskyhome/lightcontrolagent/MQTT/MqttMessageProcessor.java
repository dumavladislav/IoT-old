package org.dumskyhome.lightcontrolagent.MQTT;

import org.dumskyhome.lightcontrolagent.persistence.DAO.HomeAutomationDAO;
import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.MqttCallback;
import org.eclipse.paho.client.mqttv3.MqttMessage;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.stereotype.Component;

@Component
public class MqttMessageProcessor implements MqttCallback {

    private static final Logger LOGGER = LoggerFactory.getLogger(MqttMessageProcessor.class);

    @Autowired
    private HomeAutomationDAO homeAutomationDAO;

    @Override
    public void connectionLost(Throwable throwable) {
        LOGGER.error("connectionLost");
    }

    @Override
    public void messageArrived(String s, MqttMessage mqttMessage) throws Exception {
        LOGGER.info("MESSAGE ARRIVED!");
        byte[] payload = mqttMessage.getPayload();
        String payloadStr = new String(payload);
        int newMotionState = Integer.parseInt(payloadStr.substring(payloadStr.length()-1));     //(int) (payload[payload.length-1] & 0xFF); //intBuf.get();
        homeAutomationDAO.saveMotionEvent(newMotionState).<ResponseEntity>thenApply(ResponseEntity::ok)
                //.exceptionally(handleGetCarFailure)
        ;
    }

    @Override
    public void deliveryComplete(IMqttDeliveryToken iMqttDeliveryToken) {
        LOGGER.info("DELIVERY COMPLETE");
    }
}
