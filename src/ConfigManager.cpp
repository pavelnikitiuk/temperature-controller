#include "ConfigManager.h"

ConfigManager::ConfigManager() {
  EEPROM.begin(EEPROM_SIZE);
}

void ConfigManager::load() {
  EEPROM.get(0, config);
  Serial.print(config.magic);
  if (config.magic != CONFIG_MAGIC) {
    Serial.println("Reset eeprom");
    reset();
  }
}

void ConfigManager::save() {
  EEPROM.put(0, config);
  EEPROM.commit();
}

void ConfigManager::reset() {
  config.magic = CONFIG_MAGIC;
  strcpy(config.ssid, "");
  strcpy(config.password, "");
  strcpy(config.otaPassword, "admin");
  config.checkUpdates = false;
  config.updateInterval = 24;
  save();
}

Config* ConfigManager::getConfig() {
  return &config;
}