package org.dumskyhome.authorizationservice.persistence.datamodel;

import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.validation.constraints.NotNull;

@Entity
public class Device extends AuditModel {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private int id;

    private Long chipId;

    @NotNull
    private String macAddress;

    private String ipAddress;

    private boolean blocked;


    public Device() {}

    public Device(String macAddress, String ipAddress, Long chipId) {
        this.setMacAddress(macAddress);
        this.setIpAddress(ipAddress);
        this.setChipId(chipId);
    }


    public boolean isBlocked() {
        return blocked;
    }

    public void setBlocked(boolean blocked) {
        this.blocked = blocked;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getMacAddress() {
        return macAddress;
    }

    public void setMacAddress(String macAddress) {
        this.macAddress = macAddress;
    }

    public String getIpAddress() {
        return ipAddress;
    }

    public void setIpAddress(String ipAddress) {
        this.ipAddress = ipAddress;
    }

    public Long getChipId() {
        return chipId;
    }

    public void setChipId(Long chipId) {
        this.chipId = chipId;
    }
}
