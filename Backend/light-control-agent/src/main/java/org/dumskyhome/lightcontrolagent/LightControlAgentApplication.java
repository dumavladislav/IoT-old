package org.dumskyhome.lightcontrolagent;

import org.dumskyhome.lightcontrolagent.MQTT.MqttAgent;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.context.ApplicationContext;
import org.springframework.data.jpa.repository.config.EnableJpaAuditing;

@SpringBootApplication
@EnableJpaAuditing
public class LightControlAgentApplication {

	public static void main(String[] args) {
		ApplicationContext applicationContext = SpringApplication.run(LightControlAgentApplication.class, args);
		MqttAgent mqttAgent = applicationContext.getBean(MqttAgent.class);

		//eventsService.createTestData();

		// List<HaEvent> events = eventsService.getAllEvents();
		// System.out.println(events.size());
		// for(HaEvent event:events)
		// 	System.out.println(event.getLocation().getDescription());

//		Location location = new Location(1, "Hall");
//		eventsService.saveLocation(location);
//		eventsService.saveDevice(new Device(1,1,
//				"MC:AD:DR:ES:S0:01",
//				"192.168.1.53",
//				location));

		mqttAgent.runMqttService();
	}



}
