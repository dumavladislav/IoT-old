package org.dumskyhome.authorizationservice.service;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;
import org.dumskyhome.authorizationservice.Json.*;
import org.dumskyhome.authorizationservice.Mqtt.MqttAgent;
import org.dumskyhome.authorizationservice.persistence.DAO.AuthorizationServiceDao;
import org.dumskyhome.authorizationservice.persistence.datamodel.Device;
import org.dumskyhome.authorizationservice.persistence.datamodel.DumskyHomeSession;
import org.dumskyhome.authorizationservice.persistence.repositories.DevicesRepository;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.core.env.Environment;
import org.springframework.scheduling.annotation.Async;
import org.springframework.stereotype.Component;

import javax.persistence.criteria.CriteriaBuilder;
import java.util.concurrent.CompletableFuture;

@Component
public class AuthorizationService {

    private static final Logger logger = LoggerFactory.getLogger(AuthorizationService.class);

//    @Autowired
//    MqttAgent mqttAgent;

    @Autowired
    AuthorizationServiceDao authorizationServiceDao;

    @Autowired
    Environment env;

//    public boolean runService() {
//        if (mqttAgent.runMqttService()) logger.info("MQTT service started");
//        else logger.info("Failed to start MQTT service");
//
//        return true;
//    }

    @Async
    public CompletableFuture<JsonMqttMessageHeader> checkAuthorization(JsonMqttMessageHeader messageHeader) {
        logger.info("Checking authorization....");
        DumskyHomeSession dumskyHomeSession = authorizationServiceDao.checkAuthorization(messageHeader.getMacAddress());
        logger.info("Authorization checked");
        if (dumskyHomeSession == null) {
            authorizationServiceDao.createRegistrationRequest(messageHeader.getMacAddress());

            // TO DO: SEND MESSAGE TO REGISTRATION REQUESTS QUEUE
            logger.info("TO DO: SEND MESSAGE TO REGISTRATION REQUESTS QUEUE");


            CompletableFuture.completedFuture(messageHeader);
        }
        else
        {
            // TO DO: SEND AUTHORIZATION STATUS = authorized
            logger.info("TO DO: SEND AUTHORIZATION STATUS = authorized");
            messageHeader.setAuthorized(1);
            messageHeader.setToken(dumskyHomeSession.getSecurityToken());
//            finalizeAuthorization(messageHeader, 1);
        }
        return CompletableFuture.completedFuture(messageHeader);
    }



    public void finalizeRegistration(JsonRegistrationResponseMessage message) {
        if (message.getData().getDecision() == 1) {
            Device device = authorizationServiceDao.registerDevice(new Device(
                    message.getHeader().getMacAddress(),
                    message.getHeader().getIpAddress(),
                    message.getHeader().getChipId()
                    )
            );
            logger.info("Device registered with ID: " + device.getId());
        }
        else
        {
            logger.info("Registration DECLINE for device" + message.getHeader().getMacAddress());
        }
    }



}
