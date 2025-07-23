#include "ConfigManager.h"

ConfigManager::ConfigManager() {
  EEPROM.begin(EEPROM_SIZE);
}

void ConfigManager::load() {
  EEPROM.get(0, globalState.configuration);
  if (globalState.configuration.magic != CONFIG_MAGIC) {
    Serial.println("Reset eeprom");
    reset();
  }
}

void ConfigManager::save() {
  EEPROM.put(0, globalState.configuration);
  EEPROM.commit();
}

void ConfigManager::reset() {
  globalState.configuration.magic = CONFIG_MAGIC;
  globalState.configuration.isRelayEnabled = false;
  globalState.configuration.relayControl.mode = RELAY_CONTROL_MANUAL;
  globalState.configuration.relayControl.temperatureOn = 0;
  globalState.configuration.relayControl.temperatureOff = 0;
  strcpy(globalState.configuration.wifiState.name, "");
  strcpy(globalState.configuration.wifiState.password, "");
  strcpy(globalState.configuration.telegram.token, "");
  strcpy(globalState.configuration.telegram.chatId, "");
  strcpy(globalState.configuration.otaPassword, OTA_PASSWORD);
  save();
}