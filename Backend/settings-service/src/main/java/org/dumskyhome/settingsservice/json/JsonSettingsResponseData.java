package org.dumskyhome.settingsservice.json;

import org.dumskyhome.settingsservice.persistence.datamodel.Device;

public class JsonSettingsResponseData {
    private Device deviceSettings;

    public JsonSettingsResponseData(Device deviceSettings) {
        this.deviceSettings = deviceSettings;
    }

    public Device getDeviceSettings() {
        return deviceSettings;
    }

    public void setDeviceSettings(Device deviceSettings) {
        this.deviceSettings = deviceSettings;
    }
}
