package org.dumskyhome.authorizationservice.persistence.DAO;

import org.eclipse.paho.client.mqttv3.MqttMessage;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.scheduling.annotation.Async;
import org.springframework.scheduling.annotation.EnableAsync;
import org.springframework.stereotype.Component;

import java.util.concurrent.CompletableFuture;

import static java.lang.Thread.sleep;

@Component
public class AuthorizationServiceDao {

    private static final Logger logger = LoggerFactory.getLogger(AuthorizationServiceDao.class);

    @Async
    public CompletableFuture<Integer> checkAuthorization(String macAddress) {
        try {
            logger.info("CHECKING AUTHORIZATION: " + macAddress);
            sleep(5000);
            logger.info("AUTHORIZATION APPROVED: " + macAddress);
            return CompletableFuture.completedFuture(1);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        return CompletableFuture.completedFuture(0);
    }

}
