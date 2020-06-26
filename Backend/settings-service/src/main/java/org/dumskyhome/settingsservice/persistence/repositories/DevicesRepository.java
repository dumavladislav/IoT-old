package org.dumskyhome.settingsservice.persistence.repositories;

import org.dumskyhome.settingsservice.persistence.datamodel.Device;
import org.springframework.data.jpa.repository.JpaRepository;

public interface DevicesRepository extends JpaRepository<Device, Integer> {

    Device findByMacAddress(String macAddress);

}
