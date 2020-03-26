package org.dumskyhome.lightcontrolagent.persistence.datamodel;

import javax.persistence.*;
import javax.validation.constraints.NotNull;

@Entity
public class HaEvent extends AuditModel {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private long id;

    @NotNull
    private int eventType;

    @NotNull
    @ManyToOne(fetch = FetchType.EAGER, optional = false)
    @JoinColumn(name = "location_id", nullable = false)
    private Location location;

    @NotNull
    @ManyToOne(fetch = FetchType.EAGER, optional = false)
    @JoinColumn(name = "device_id", nullable = false)
    private Device device;

    public HaEvent() {}

    public HaEvent(@NotNull int eventType, @NotNull Location location, @NotNull Device device) {
        this.eventType = eventType;
        this.location = location;
        this.device = device;
    }

    public int getEventType() {
        return eventType;
    }

    public void setEventType(int eventType) {
        this.eventType = eventType;
    }

    public Location getLocation() {
        return location;
    }

    public void setLocation(Location location) {
        this.location = location;
    }

    public Device getDevice() {
        return device;
    }

    public void setDevice(Device device) {
        this.device = device;
    }
}
