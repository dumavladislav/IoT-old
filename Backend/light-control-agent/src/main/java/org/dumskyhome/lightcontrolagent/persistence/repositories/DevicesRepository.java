package org.dumskyhome.lightcontrolagent.persistence.repositories;

import org.dumskyhome.lightcontrolagent.persistence.datamodel.Device;
import org.springframework.data.jpa.repository.JpaRepository;

public interface DevicesRepository extends JpaRepository<Device, Integer> {
}
