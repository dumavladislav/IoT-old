package org.dumskyhome.gpsagent.mqtt;

import com.fasterxml.jackson.databind.DeserializationFeature;
import com.fasterxml.jackson.databind.ObjectMapper;
import org.dumskyhome.gpsagent.json.JsonGpsDataMessage;
import org.dumskyhome.gpsagent.persistence.dao.GpsAgentDao;
import org.dumskyhome.gpsagent.persistence.datamodel.GpsData;
import org.dumskyhome.gpsagent.service.GPSAgentService;
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

@Component
@PropertySource("classpath:mqtt.properties")
public class MqttMessageProcessor implements MqttCallback {

    private static final Logger logger = LoggerFactory.getLogger(MqttMessageProcessor.class);
    private ObjectMapper objectMapper;

    public MqttMessageProcessor() {
        objectMapper = new ObjectMapper();
        objectMapper.disable(DeserializationFeature.FAIL_ON_UNKNOWN_PROPERTIES);
        objectMapper.enable(DeserializationFeature.ACCEPT_EMPTY_STRING_AS_NULL_OBJECT);
        objectMapper.disable(DeserializationFeature.FAIL_ON_MISSING_CREATOR_PROPERTIES);
        objectMapper.disable(DeserializationFeature.FAIL_ON_IGNORED_PROPERTIES);
    }

    @Autowired
    private Environment env;

    @Autowired
    //private GPSAgentService gpsAgentService;
    private GpsAgentDao gpsAgentDao;

    @Override
    public void connectionLost(Throwable throwable) {
        logger.error("CONNECTION LOST");
    }

    @Override
    public void messageArrived(String s, MqttMessage mqttMessage) throws Exception {
        // ObjectMapper objectMapper = new ObjectMapper();

        logger.info("TOPIC:"+ s + " || MESSAGE RECEIVED: " + mqttMessage.toString());
        if (s.equals(env.getProperty("mqtt.topic.gpsData"))) {
            logger.info("GPS Data received. Parsing....");
            JsonGpsDataMessage jsonGpsDataMessage = objectMapper.readValue(mqttMessage.toString(), JsonGpsDataMessage.class);

            logger.info(jsonGpsDataMessage.getData().getLat().toString());
            //gpsAgentService.saveGpsData(jsonGpsDataMessage).<ResponseEntity>thenApply(ResponseEntity::ok);
            GpsData gpsData = gpsAgentDao.saveGpsData(new GpsData(
                    jsonGpsDataMessage.getHeader().getMacAddress(),
                    jsonGpsDataMessage.getData().getMillis(),
                    jsonGpsDataMessage.getData().getLat(),
                    jsonGpsDataMessage.getData().getLng()
            ));

        }
    }

    @Override
    public void deliveryComplete(IMqttDeliveryToken iMqttDeliveryToken) {
        logger.info("DELIVERY COMPLETE");
    }

}
