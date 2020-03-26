package org.dumskyhome.lightcontrolagent.persistence.services;

import org.dumskyhome.lightcontrolagent.MQTT.MqttAgent;
import org.dumskyhome.lightcontrolagent.persistence.datamodel.Device;
import org.dumskyhome.lightcontrolagent.persistence.datamodel.HaEvent;
import org.dumskyhome.lightcontrolagent.persistence.datamodel.Location;
import org.dumskyhome.lightcontrolagent.persistence.datamodel.MotionEvent;
import org.dumskyhome.lightcontrolagent.persistence.repositories.DevicesRepository;
import org.dumskyhome.lightcontrolagent.persistence.repositories.EventsRepository;
import org.dumskyhome.lightcontrolagent.persistence.repositories.LocationsRepository;
import org.dumskyhome.lightcontrolagent.persistence.repositories.MotionEventsRepository;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.scheduling.annotation.Async;
import org.springframework.stereotype.Component;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.util.List;
import java.util.Optional;
import java.util.concurrent.CompletableFuture;

import static java.lang.Thread.sleep;

@Service
public class EventsService {

    private static final Logger LOGGER = LoggerFactory.getLogger(EventsService.class);

    @Autowired
    private MqttAgent mqttAgent;

    @Autowired
    private EventsRepository eventsRepository;

    @Autowired
    private LocationsRepository locationsRepository;

    @Autowired
    private DevicesRepository devicesRepository;

    @Autowired
    private MotionEventsRepository motionsEventRepository;

    public void saveLocation(Location location) {
        locationsRepository.save(location);
    }

    public void saveDevice(Device device) {
        devicesRepository.save(device);
    }

    public List<HaEvent> getAllEvents() {
        return eventsRepository.findAll();
    }

    public void saveEvent(HaEvent event) { eventsRepository.save(event);}

    public void runMqttService() {
        mqttAgent.connect();
        if (mqttAgent.isConnected()) {
            mqttAgent.subscribeToHaEvents(null);
        }
    }

    /*public long saveMotionEvent(int state) {
        LOGGER.info("Save Motion Event to the DB");
        try {
            sleep(10000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        Optional<Location> location = locationsRepository.findById(1);
        Optional<Device> device = devicesRepository.findById(1);
        HaEvent haEvent = new HaEvent(1,
                location.get(),
                device.get());
        eventsRepository.save(haEvent);

        MotionEvent motionEvent = new MotionEvent(haEvent);
        motionEvent.setMotionSensorState(state);
        LOGGER.info("Motion Event prepared for save!");
        motionEvent = motionsEventRepository.save(motionEvent);
        return motionEvent.getId();
    }*/


    @Async
    public CompletableFuture<Long> saveMotionEvent(int state) {
        LOGGER.info("Save Motion Event to the DB");
        Optional<Location> location = locationsRepository.findById(1);
        Optional<Device> device = devicesRepository.findById(1);
        HaEvent haEvent = new HaEvent(1,
                location.get(),
                device.get());
        eventsRepository.save(haEvent);

        MotionEvent motionEvent = new MotionEvent(haEvent);
        motionEvent.setMotionSensorState(state);
        LOGGER.info("Motion Event prepared for save!");
        motionEvent = motionsEventRepository.save(motionEvent);
        return CompletableFuture.completedFuture(motionEvent.getId());
    }


    public void createTestData() {

        Location location = new Location();

        location.setDescription("Hall");
        saveLocation(location);

        Device dev1 = new Device(122, "qD:45:76:32", "192.168.1.53", location);
        //dev1.setIpAddress("192.168.1.53");
        //dev1.setMacAddress("qD:45:76:32");
        //dev1.setRegistrationDecision(123);
        //dev1.setLocaion(location);
        saveDevice(dev1);

        Device dev2 = new Device(122, "qD:45:80:32", "192.168.1.55", location);
//        dev2.setIpAddress("192.168.1.58");
//        dev2.setMacAddress("qD:45:80:45");
//        dev2.setRegistrationDecision(124);
//        dev2.setLocaion(location);
        saveDevice(dev2);

        HaEvent event = new HaEvent();
        event.setLocation(location);
        event.setDevice(dev1);
        event.setEventType(1);
        saveEvent(event);

        HaEvent event1 = new HaEvent();
        event1.setLocation(location);
        event1.setDevice(dev2);
        event1.setEventType(2);
        saveEvent(event1);

        HaEvent event2 = new HaEvent();
        event1.setLocation(location);
        event1.setDevice(dev1);
        event1.setEventType(1);
        saveEvent(event1);


    }
}
