package org.dumskyhome.settingsservice;

import org.dumskyhome.settingsservice.service.SettingsService;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.context.ApplicationContext;

@SpringBootApplication
public class SettingsServiceApplication {

	private static final Logger logger = LoggerFactory.getLogger(SettingsServiceApplication.class);

	public static void main(String[] args) {
		// SpringApplication.run(SettingsServiceApplication.class, args);
		ApplicationContext applicationContext = SpringApplication.run(SettingsServiceApplication.class);
		logger.info("========= Application started =========");

		SettingsService settingsService = applicationContext.getBean(SettingsService.class);
		while(!settingsService.runService()) {
			logger.error("Failed to start Settings service");
			try {
				Thread.sleep(5000);
				settingsService.runService();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}

		logger.info("========= Settings service started =========");

	}

}
