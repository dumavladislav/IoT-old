#pragma once
#include <Arduino.h>

struct AuthorizationBlock {
    String deviceId;
    String macAddress;
    String ipAddress;
    uint32_t chipId;

    // ------------------------ Authorization ----------------------------
    boolean authorized;
    String securityToken;   
};