#include <ArduinoOTA.h>
#include <ESP8266mDNS.h>

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