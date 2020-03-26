package org.dumskyhome.lightcontrolagent.persistence.repositories;

import org.dumskyhome.lightcontrolagent.persistence.datamodel.MotionEvent;
import org.springframework.data.jpa.repository.JpaRepository;

public interface MotionEventsRepository extends JpaRepository<MotionEvent, Long> {
}
