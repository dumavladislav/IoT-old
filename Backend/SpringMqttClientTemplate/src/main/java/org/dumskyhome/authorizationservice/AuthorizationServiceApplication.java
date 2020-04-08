package org.dumskyhome.authorizationservice;

import org.dumskyhome.authorizationservice.Mqtt.MqttAgent;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.context.ApplicationContext;

@SpringBootApplication
public class AuthorizationServiceApplication {

	private static final Logger logger = LoggerFactory.getLogger(AuthorizationServiceApplication.class);

	public static void main(String[] args) {
		ApplicationContext applicationContext = SpringApplication.run(AuthorizationServiceApplication.class, args);
		logger.info("Application started");

		MqttAgent mqttAgent = applicationContext.getBean(MqttAgent.class);
		if (mqttAgent.runMqttService()) logger.info("MQTT service started");
		else logger.info("Failed to start MQTT service");
	}
}
