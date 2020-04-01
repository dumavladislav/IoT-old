struct ModeSettings_MSDRIVEN {
    int keepOnDurationSec = 10;
};

struct ModeSettings_ON {
    int maxOnOperationModeDuration = 60;
};

struct MotionSensorSettings {
    int relayMode = 0;
    ModeSettings_MSDRIVEN msDriveModeSettings;
    ModeSettings_ON onModeSettings;  
};