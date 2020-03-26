package org.dumskyhome.lightcontrolagent;

import org.dumskyhome.lightcontrolagent.MQTT.MqttAgent;
import org.dumskyhome.lightcontrolagent.persistence.datamodel.Device;
import org.dumskyhome.lightcontrolagent.persistence.datamodel.HaEvent;
import org.dumskyhome.lightcontrolagent.persistence.datamodel.Location;
import org.dumskyhome.lightcontrolagent.persistence.services.EventsService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.context.ApplicationContext;
import org.springframework.data.jpa.repository.config.EnableJpaAuditing;

import java.util.List;

@SpringBootApplication
@EnableJpaAuditing
public class LightControlAgentApplication {

	public static void main(String[] args) {
		ApplicationContext applicationContext = SpringApplication.run(LightControlAgentApplication.class, args);
		EventsService eventsService = applicationContext.getBean(EventsService.class);

		//eventsService.createTestData();

		// List<HaEvent> events = eventsService.getAllEvents();
		// System.out.println(events.size());
		// for(HaEvent event:events)
		// 	System.out.println(event.getLocation().getDescription());

		Location location = new Location(1, "Hall");
		eventsService.saveLocation(location);
		eventsService.saveDevice(new Device(1,1,
				"MC:AD:DR:ES:S0:01",
				"192.168.1.53",
				location));

		eventsService.runMqttService();
	}



}
