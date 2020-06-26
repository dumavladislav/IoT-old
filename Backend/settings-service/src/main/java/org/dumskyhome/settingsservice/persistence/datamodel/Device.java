package org.dumskyhome.settingsservice.persistence.datamodel;

import com.fasterxml.jackson.annotation.JsonBackReference;
import com.fasterxml.jackson.annotation.JsonIgnore;
import com.fasterxml.jackson.annotation.JsonManagedReference;
import com.sun.istack.NotNull;

import javax.persistence.*;
import java.util.Set;

@Entity
public class Device {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    @JsonIgnore
    private int id;

    @JsonIgnore
    private Long chipId;

    @NotNull
    @JsonIgnore
    private String macAddress;

    @JsonIgnore
    private String ipAddress;

//    @JsonIgnore
    @OneToMany(mappedBy = "device", fetch = FetchType.EAGER, cascade = CascadeType.ALL)
    private Set<SettingsPreset> settingsPresets;

    public Device() {}

    public Device(int id, @NotNull String macAddress, String ipAddress, Long chipId) {
        this(macAddress, ipAddress, chipId);
        this.id = id;

    }

    public Device(@NotNull String macAddress, String ipAddress, Long chipId) {
        this.macAddress = macAddress;
        this.ipAddress = ipAddress;
        this.chipId = chipId;
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

    public Set<SettingsPreset> getSettingsPresets() {
        return settingsPresets;
    }

    public void setSettingsPresets(Set<SettingsPreset> settingsPresets) {
        this.settingsPresets = settingsPresets;
    }

    public Long getChipId() {
        return chipId;
    }

    public void setChipId(Long chipId) {
        this.chipId = chipId;
    }
}
