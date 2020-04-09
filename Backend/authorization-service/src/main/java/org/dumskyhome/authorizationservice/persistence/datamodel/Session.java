package org.dumskyhome.authorizationservice.persistence.datamodel;

import javax.persistence.*;

@Entity
public class Session extends AuditModel {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private long id;

    @OneToOne
    @JoinColumn(name = "deviceId")
    private Device device;

    private String securityToken;

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
