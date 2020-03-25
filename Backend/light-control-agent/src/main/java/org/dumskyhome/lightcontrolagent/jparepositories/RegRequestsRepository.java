package org.dumskyhome.lightcontrolagent.jparepositories;

import org.dumskyhome.lightcontrolagent.datamodel.RegistrationRequest;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;

@Repository
public interface RegRequestsRepository extends JpaRepository<RegistrationRequest, Long> {
}
