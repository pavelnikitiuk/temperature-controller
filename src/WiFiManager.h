// WiFiManager.h
#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <ESP8266WiFi.h>
#include "ConfigManager.h"

class WiFiManager {
  public:
    WiFiManager(ConfigManager &config);
    bool begin();
    bool connect();
    void startAP();

  private:
    ConfigManager &configManager;
};

#endif