package org.dumskyhome.lightcontrolagent.persistence.datamodel;

import javax.persistence.*;
import javax.validation.constraints.NotNull;

@Entity
public class Device {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private int id;

    private long registrationDecision;

    @NotNull
    private String macAddress;

    private String ipAddress;

    @OneToOne(fetch = FetchType.LAZY)
    @JoinColumn(name = "locationId")
    private Location locaion;

    public Device() {}

    public Device(int id, long registrationDecision, @NotNull String macAddress, String ipAddress, Location locaion) {
        this(registrationDecision, macAddress, ipAddress, locaion);
        this.id = id;

    }

    public Device(long registrationDecision, @NotNull String macAddress, String ipAddress, Location locaion) {
        this.registrationDecision = registrationDecision;
        this.macAddress = macAddress;
        this.ipAddress = ipAddress;
        this.locaion = locaion;
    }



    public long getRegistrationDecision() {
        return registrationDecision;
    }

    public void setRegistrationDecision(long registrationDecision) {
        this.registrationDecision = registrationDecision;
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

    public Location getLocaion() {
        return locaion;
    }

    public void setLocaion(Location locaion) {
        this.locaion = locaion;
    }
}
