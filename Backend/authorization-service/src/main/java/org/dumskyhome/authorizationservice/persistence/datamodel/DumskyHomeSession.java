package org.dumskyhome.authorizationservice.persistence.datamodel;

import javax.persistence.*;
import java.util.UUID;

@Entity
public class DumskyHomeSession extends AuditModel {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private long id;

    @OneToOne
    @JoinColumn(name = "deviceId")
    private Device device;

    private String securityToken;

    public DumskyHomeSession() {
        createSession();
    };

    public DumskyHomeSession(Device device) {
        this.device = device;
        createSession();
    };

    public String createSession() {
        securityToken = UUID.randomUUID().toString();
        return securityToken;
    }

    public long getId() {
        return id;
    }

    public void setId(long id) {
        this.id = id;
    }

    public Device getDevice() {
        return device;
    }

    public void setDevice(Device device) {
        this.device = device;
    }

    public String getSecurityToken() {
        return securityToken;
    }

    public void setSecurityToken(String securityToken) {
        this.securityToken = securityToken;
    }
}
