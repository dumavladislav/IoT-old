package org.dumskyhome.authorizationservice.service;

import org.dumskyhome.authorizationservice.Mqtt.MqttAgent;
import org.dumskyhome.authorizationservice.persistence.DAO.AuthorizationServiceDao;
import org.dumskyhome.authorizationservice.persistence.datamodel.DumskyHomeSession;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.scheduling.annotation.Async;
import org.springframework.stereotype.Component;

import java.util.concurrent.CompletableFuture;

@Component
public class AuthorizationService {

    private static final Logger logger = LoggerFactory.getLogger(AuthorizationService.class);

    @Autowired
    MqttAgent mqttAgent;

    @Autowired
    AuthorizationServiceDao authorizationServiceDao;

    public boolean runService() {
        if (mqttAgent.runMqttService()) logger.info("MQTT service started");
        else logger.info("Failed to start MQTT service");

        return true;
    }

    @Async
    public CompletableFuture<Integer> checkAuthorization(String macAddress) {
        DumskyHomeSession dumskyHomeSession = authorizationServiceDao.checkAuthorization(macAddress);
        if (dumskyHomeSession == null) {
            authorizationServiceDao.createRegistrationRequest(macAddress);

            // TO DO: SEND MESSAGE TO REGISTRATION REQUESTS QUEUE
            logger.info("TO DO: SEND MESSAGE TO REGISTRATION REQUESTS QUEUE");

            CompletableFuture.completedFuture(-1);
        }
        else
        {
            // TO DO: SEND AUTHORIZATION STATUS = authorized
            logger.info("TO DO: SEND AUTHORIZATION STATUS = authorized");
        }
        return CompletableFuture.completedFuture(1);
    }

}
