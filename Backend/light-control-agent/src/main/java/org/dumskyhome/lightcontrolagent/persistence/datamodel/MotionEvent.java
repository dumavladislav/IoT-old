package org.dumskyhome.lightcontrolagent.persistence.datamodel;

import javax.persistence.*;

@Entity
public class MotionEvent {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private long id;

    @OneToOne(fetch = FetchType.EAGER, optional = false)
    @JoinColumn(name = "event_id")
    private HaEvent haEvent;

    private int motionSensorState;

    public MotionEvent() {

    }

    public MotionEvent(HaEvent haEvent) {
        this.haEvent = haEvent;
    }

    public long getId() {
        return id;
    }

    public void setId(long id) {
        this.id = id;
    }

    public HaEvent getHaEvent() {
        return haEvent;
    }

    public void setHaEvent(HaEvent haEvent) {
        this.haEvent = haEvent;
    }

    public int getMotionSensorState() {
        return motionSensorState;
    }

    public void setMotionSensorState(int motionSensorState) {
        this.motionSensorState = motionSensorState;
    }
}
