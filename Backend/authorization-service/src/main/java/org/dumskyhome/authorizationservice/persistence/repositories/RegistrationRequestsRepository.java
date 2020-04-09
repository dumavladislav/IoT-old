package org.dumskyhome.authorizationservice.persistence.repositories;

import org.dumskyhome.authorizationservice.persistence.datamodel.RegistrationRequest;
import org.springframework.data.jpa.repository.JpaRepository;

public interface RegistrationRequestsRepository extends JpaRepository<RegistrationRequest, Long> {
}
