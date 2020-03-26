package org.dumskyhome.lightcontrolagent.persistence.DAO;

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

import java.util.List;
import java.util.Optional;
import java.util.concurrent.CompletableFuture;

@Component
public class HomeAutomationDAO {

    private static final Logger LOGGER = LoggerFactory.getLogger(HomeAutomationDAO.class);

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


}
