package org.dumskyhome.authorizationservice.persistence.repositories;

import org.dumskyhome.authorizationservice.persistence.datamodel.Device;
import org.springframework.data.jpa.repository.JpaRepository;

import java.util.List;

public interface DevicesRepository extends JpaRepository<Device, Integer> {

    List<Device> findByMacAddress(String macAddress);
}
