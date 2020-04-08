package org.dumskyhome.authorizationservice.Mqtt;

import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.context.annotation.PropertySource;
import org.springframework.core.env.Environment;
import org.springframework.stereotype.Component;

import java.util.concurrent.Executors;
import java.util.concurrent.ThreadPoolExecutor;

@Component
@PropertySource("classpath:mqtt.properties")
public class MqttAgent {

    private static final Logger logger = LoggerFactory.getLogger(MqttAgent.class);

    private MqttClient mqttClient;

    @Value("${mqtt.clientId}")
    private String mqttClientId;
    //private ThreadPoolExecutor executor;
    @Autowired
    MqttMessageProcessor mqttMessageProcessor;

    @Autowired
    Environment env;

    private boolean connect() {

        try {
            mqttClient = new MqttClient(env.getProperty("mqtt.serverUrl"), mqttClientId);
            MqttConnectOptions mqttConnectOptions = new MqttConnectOptions();
            mqttConnectOptions.setUserName(env.getProperty("mqtt.user"));
            mqttConnectOptions.setPassword(env.getProperty("mqtt.password").toCharArray());
            mqttConnectOptions.setConnectionTimeout(Integer.parseInt(env.getProperty("mqtt.connectionTimeout")));
            mqttConnectOptions.setCleanSession(true);

            mqttClient.connect(mqttConnectOptions);
            //if (mqttClient.isConnected()) executor = (ThreadPoolExecutor) Executors.newFixedThreadPool(3);
            return mqttClient.isConnected();

        } catch (MqttException e) {
            e.printStackTrace();
        }
        return false;
    }

    public boolean runMqttService() {
        if(connect()) {
            logger.info("Connected to MQTT");
            try {
                mqttClient.subscribe(env.getProperty("mqtt.topic.authorizationRequests"));
                mqttClient.setCallback(mqttMessageProcessor);
                return true;
            } catch (MqttException e) {
                e.printStackTrace();
            }
        }
        return false;
    }

}
