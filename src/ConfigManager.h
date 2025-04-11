#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <EEPROM.h>
#include <ESP8266WiFi.h>

#include "Variables.h"

struct Config {
  uint32_t magic;
  char ssid[32];
  char password[64];
  char otaPassword[32];
  bool checkUpdates;
  int updateInterval;
};

class ConfigManager {
  public:
    ConfigManager();
    void load();
    void save();
    void reset();
    Config* getConfig();

  private:
    Config config;
};

#endif