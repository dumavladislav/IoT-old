package org.dumskyhome.authorizationservice.persistence.datamodel;

import com.fasterxml.jackson.annotation.JsonTypeInfo;

import javax.persistence.*;

@Entity
public class RegistrationDecision extends AuditModel {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private long id;

    @OneToOne
    @JoinColumn(name = "regRequestId")
    private RegistrationRequest registrationRequest;

    private boolean decision;     // 1 - Approve; 2 - decline

}
