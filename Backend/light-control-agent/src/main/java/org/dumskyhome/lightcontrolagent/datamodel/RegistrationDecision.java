package org.dumskyhome.lightcontrolagent.datamodel;

import javax.persistence.*;
import javax.validation.constraints.NotBlank;

@Entity(name = "registration_decision")
public class RegistrationDecision extends AuditModel{
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private long id;

    @ManyToOne(fetch = FetchType.LAZY, optional = false)
    @JoinColumn(name = "reg_request_id", nullable = false)
    private RegistrationRequest request;

    @NotBlank
    private int decision;
}
