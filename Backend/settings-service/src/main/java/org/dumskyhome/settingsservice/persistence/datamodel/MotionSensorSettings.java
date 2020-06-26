package org.dumskyhome.settingsservice.persistence.datamodel;

import com.fasterxml.jackson.annotation.JsonIgnore;

import javax.persistence.*;

@Entity
public class MotionSensorSettings extends AuditModel {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    @JsonIgnore
    private int id;

    private int relayMode;
    private int msDriveModeDuration;
    private int onModeDuration;

    @JsonIgnore
    @OneToOne
    @JoinColumn(name = "settings_preset_id")
    private SettingsPreset settingsPreset;

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public int getRelayMode() {
        return relayMode;
    }

    public void setRelayMode(int relayMode) {
        this.relayMode = relayMode;
    }

    public int getMsDriveModeDuration() {
        return msDriveModeDuration;
    }

    public void setMsDriveModeDuration(int msDriveModeDuration) {
        this.msDriveModeDuration = msDriveModeDuration;
    }

    public int getOnModeDuration() {
        return onModeDuration;
    }

    public void setOnModeDuration(int onModeDuration) {
        this.onModeDuration = onModeDuration;
    }

    public SettingsPreset getSettingsPreset() {
        return settingsPreset;
    }

    public void setSettingsPreset(SettingsPreset settingsPreset) {
        this.settingsPreset = settingsPreset;
    }
}
