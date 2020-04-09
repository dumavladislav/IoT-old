package org.dumskyhome.authorizationservice.persistence.DAO;

import org.dumskyhome.authorizationservice.persistence.datamodel.Device;
import org.dumskyhome.authorizationservice.persistence.repositories.RegistrationRequestsRepository;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.scheduling.annotation.Async;
import org.springframework.stereotype.Component;

import java.util.List;
import java.util.concurrent.CompletableFuture;

@Component
public class AuthorizationServiceDao {

    private static final Logger logger = LoggerFactory.getLogger(AuthorizationServiceDao.class);

    @Autowired
    private RegistrationRequestsRepository registrationRequestsRepository;

    @Async
    public CompletableFuture<Integer> checkAuthorization(String macAddress) {
        logger.info("CHECKING AUTHORIZATION: " + macAddress);
        List<Device> deiviceList = registrationRequestsRepository.findByMacAddress(macAddress);
        if (deiviceList.size()>0) {
            createSession(deiviceList.get(0));
        }
        else {
            createRegistrationRequest(macAddress);
        }
        //sleep(5000);
        logger.info("AUTHORIZATION APPROVED: " + macAddress);
        return CompletableFuture.completedFuture(1);
        //return CompletableFuture.completedFuture(0);
    }

    private void createRegistrationRequest(String macAddress) {
        logger.info("Creating registration request for " + macAddress);
    }

    private void createSession(Device device) {
        logger.info("Creating session for device id: " + device.getId());
    }

}
