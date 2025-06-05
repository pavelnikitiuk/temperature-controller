#ifndef OTA_UPDATER_H
#define OTA_UPDATER_H

#include <ArduinoOTA.h>

#ifdef ESP8266
  #include <ESP8266mDNS.h>
#elif defined(ESP32)
  #include <ESPmDNS.h>
#endif

#include "ConfigManager.h"
#include "DisplayManager.h"

class OTAUpdater {
private:
  ConfigManager &configManager;
  DisplayManager &displayManager;
  void handleStart();
  void handleEnd();
  void handleError(ota_error_t error);

public:
  OTAUpdater(ConfigManager &config, DisplayManager &display);
  void setup();
  void handle();
};

#endif
