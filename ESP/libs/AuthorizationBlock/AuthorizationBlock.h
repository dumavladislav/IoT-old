#pragma once

#include <Arduino.h>
#include <iostream>

struct AuthorizationBlock {
    String deviceId;
    String macAddress;
    String ipAddress;
    uint32_t chipId;

    // ------------------------ Authorization ----------------------------
    boolean authorized;
    String securityToken;   
};