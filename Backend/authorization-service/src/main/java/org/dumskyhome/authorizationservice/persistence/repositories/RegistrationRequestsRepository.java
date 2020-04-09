package org.dumskyhome.authorizationservice.persistence.repositories;

import org.dumskyhome.authorizationservice.persistence.datamodel.Device;
import org.dumskyhome.authorizationservice.persistence.datamodel.RegistrationRequest;
import org.springframework.data.jpa.repository.JpaRepository;

import java.util.List;

public interface RegistrationRequestsRepository extends JpaRepository<RegistrationRequest, Long> {

    List<Device> findByMacAddress(String macAddress);
}
