package org.dumskyhome.settingsservice.persistence.datamodel;

import com.fasterxml.jackson.annotation.JsonBackReference;
import com.fasterxml.jackson.annotation.JsonIgnore;
import com.fasterxml.jackson.annotation.JsonIgnoreProperties;
import com.fasterxml.jackson.annotation.JsonManagedReference;

import javax.persistence.*;

@Entity
@JsonIgnoreProperties(value= {"device"})
public class SettingsPreset extends AuditModel {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    @JsonIgnore
    private int id;

    private String name;
    private boolean defaultPreset;
    @Column(columnDefinition = "int4 default 0")
    private int operationMode;

    @JsonIgnore
    @ManyToOne(fetch = FetchType.LAZY)
    @JoinColumn(name="device_id", nullable = false)
    private Device device;

    @OneToOne(mappedBy = "settingsPreset", fetch = FetchType.EAGER, optional = false)
    private MotionSensorSettings motionSensorSettings;

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public boolean isDefaultPreset() {
        return defaultPreset;
    }

    public void setDefaultPreset(boolean defaultPreset) {
        this.defaultPreset = defaultPreset;
    }

    public Device getDevice() {
        return device;
    }

    public void setDevice(Device device) {
        this.device = device;
    }

    public MotionSensorSettings getMotionSensorSettings() {
        return motionSensorSettings;
    }

    public void setMotionSensorSettings(MotionSensorSettings motionSensorSettings) {
        this.motionSensorSettings = motionSensorSettings;
    }

    public int getOperationMode() {
        return operationMode;
    }

    public void setOperationMode(int operationMode) {
        this.operationMode = operationMode;
    }
}
