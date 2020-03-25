package org.dumskyhome.lightcontrolagent.jparepositories;

import org.dumskyhome.lightcontrolagent.datamodel.RegistrationDecision;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;

import java.util.List;

@Repository
public interface RegDecisionsRepository extends JpaRepository<RegDecisionsRepository, Long> {
    List<RegistrationDecision> findByRequestId(Long regRequestId);
}
