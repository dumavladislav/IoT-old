package org.dumskyhome.settingsservice.service;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;
import org.dumskyhome.settingsservice.json.JsonMqttMessageHeader;
import org.dumskyhome.settingsservice.json.JsonSettingsResponseData;
import org.dumskyhome.settingsservice.json.JsonSettingsResponseMessage;
import org.dumskyhome.settingsservice.mqtt.MqttAgent;
import org.dumskyhome.settingsservice.persistence.dao.SettingsServiceDao;
import org.dumskyhome.settingsservice.persistence.datamodel.Device;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.annotation.PropertySource;
import org.springframework.core.env.Environment;
import org.springframework.scheduling.annotation.Async;
import org.springframework.stereotype.Component;

import java.util.concurrent.CompletableFuture;

@Component
@PropertySource("classpath:mqtt.properties")
public class SettingsService {

    private static final Logger logger = LoggerFactory.getLogger(SettingsService.class);

    @Autowired
    MqttAgent mqttAgent;

    @Autowired
    private SettingsServiceDao settingsServiceDao;

    @Autowired
    private Environment env;

    public boolean runService() {
        return mqttAgent.runMqttService();
    }

    @Async
    public CompletableFuture<Boolean> provideDeviceSettings(JsonMqttMessageHeader messageHeader) {

        ObjectMapper objectMapper = new ObjectMapper();

        Device deviceSettings = settingsServiceDao.getDeviceSettings(messageHeader.getMacAddress());
        if (deviceSettings == null) {

            logger.warn("UNKNOWN DEVICE");
            deviceSettings = new Device(messageHeader.getMacAddress(), messageHeader.getIpAddress(), messageHeader.getChipId());
//            logger.info(messageHeader.getMacAddress());
//            deviceSettings.setMacAddress(messageHeader.getMacAddress());
//            logger.info(messageHeader.getIpAddress());
//            deviceSettings.setIpAddress(messageHeader.getIpAddress());
//            logger.info(messageHeader.getChipId().toString());
//            deviceSettings.setChipId(messageHeader.getChipId());

            logger.warn("SAVING UNKNOWN DEVICE");

            settingsServiceDao.saveDevice(deviceSettings);
        }

        try {
            JsonSettingsResponseMessage jsonSettingsResponseMessage = new JsonSettingsResponseMessage();
            jsonSettingsResponseMessage.setHeader(messageHeader);
            jsonSettingsResponseMessage.setData(new JsonSettingsResponseData(deviceSettings));
            String message = objectMapper.writeValueAsString(jsonSettingsResponseMessage);
            mqttAgent.sendMessage(env.getProperty("mqtt.topic.settingsApply"), message, 2);
            return CompletableFuture.completedFuture(true);
        } catch (JsonProcessingException e) {
            e.printStackTrace();
        }
        return CompletableFuture.completedFuture(false);
    };

}
