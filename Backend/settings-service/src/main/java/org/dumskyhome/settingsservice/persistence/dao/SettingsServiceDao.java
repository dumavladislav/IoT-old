package org.dumskyhome.settingsservice.persistence.dao;

import org.dumskyhome.settingsservice.persistence.datamodel.Device;
import org.dumskyhome.settingsservice.persistence.repositories.DevicesRepository;
import org.dumskyhome.settingsservice.service.SettingsService;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

@Component
public class SettingsServiceDao {

    private static final Logger logger = LoggerFactory.getLogger(SettingsServiceDao.class);

    @Autowired
    private DevicesRepository devicesRepository;

    public Device getDeviceSettings(String macAddress) {
        return devicesRepository.findByMacAddress(macAddress);
    }

    public Device saveDevice(Device device) {
        logger.warn("Creating new device: " + device.getMacAddress());
        return devicesRepository.save(device);
    }

}
