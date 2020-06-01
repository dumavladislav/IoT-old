package org.dumskyhome.authorizationservice.persistence.DAO;

import org.dumskyhome.authorizationservice.persistence.datamodel.Device;
import org.dumskyhome.authorizationservice.persistence.datamodel.DumskyHomeSession;
import org.dumskyhome.authorizationservice.persistence.datamodel.RegistrationRequest;
import org.dumskyhome.authorizationservice.persistence.repositories.DevicesRepository;
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

    @Autowired
    private DevicesRepository devicesRepository;

    public DumskyHomeSession checkAuthorization(String macAddress) {
        logger.info("CHECKING AUTHORIZATION: " + macAddress);
        List<Device> deiviceList = devicesRepository.findByMacAddress(macAddress);
        logger.info("Found " + deiviceList.size() + " devices");
        if (deiviceList.size()>0) {
            return createSession(deiviceList.get(0));
        }
        //sleep(5000);
        return null;
    }

    public void createRegistrationRequest(String macAddress) {
        logger.info("Creating registration request for " + macAddress);
        RegistrationRequest registrationRequest= new RegistrationRequest(macAddress);
        registrationRequestsRepository.save(registrationRequest);
    }

    public Device registerDevice(Device device) {
        logger.info("Registering device: " + device.getMacAddress());
        return devicesRepository.save(device);
    }

    private DumskyHomeSession createSession(Device device) {
        logger.info("Creating session for device id: " + device.getId());
        return new DumskyHomeSession(device);
    }

}
