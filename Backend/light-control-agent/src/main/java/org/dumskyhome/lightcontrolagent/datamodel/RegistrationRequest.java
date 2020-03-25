package org.dumskyhome.lightcontrolagent.datamodel;

import javax.persistence.*;
import javax.validation.constraints.NotBlank;

@Entity
@Table(name = "registration_requests")
public class RegistrationRequest extends AuditModel {
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private long id;

    @NotBlank
    private String deviceMacAddress;
}
