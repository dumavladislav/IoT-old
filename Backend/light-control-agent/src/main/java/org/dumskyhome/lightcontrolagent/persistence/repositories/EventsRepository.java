package org.dumskyhome.lightcontrolagent.persistence.repositories;

import org.dumskyhome.lightcontrolagent.persistence.datamodel.HaEvent;
import org.springframework.data.jpa.repository.JpaRepository;

public interface EventsRepository extends JpaRepository<HaEvent, Long> {
}
