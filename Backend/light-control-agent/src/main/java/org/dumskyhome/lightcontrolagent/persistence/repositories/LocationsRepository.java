package org.dumskyhome.lightcontrolagent.persistence.repositories;

import org.dumskyhome.lightcontrolagent.persistence.datamodel.Location;
import org.springframework.data.jpa.repository.JpaRepository;

public interface LocationsRepository extends JpaRepository<Location, Integer> {
}
