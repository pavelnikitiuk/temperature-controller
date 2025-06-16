#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <EEPROM.h>
#if defined(ESP8266)
  #include <ESP8266WiFi.h>
#elif defined(ESP32)
  #include <WiFi.h>
#endif

#include "Variables.h"

class ConfigManager {
  public:
    ConfigManager();
    void load();
    void save();
    void reset();
};

#endif