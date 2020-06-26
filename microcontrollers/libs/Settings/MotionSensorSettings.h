/*struct ModeSettings_MSDRIVEN {
    int keepOnDurationSec = 20;
};

struct ModeSettings_ON {
    int maxOnOperationModeDuration = 60;
};*/

struct MotionSensorSettings {
    int relayMode = 0;
    int msDriveModeDuration = 20;
    int onModeDuration = 1800;  
};