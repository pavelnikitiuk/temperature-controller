// OTAUpdater.cpp
#include "OTAUpdater.h"

OTAUpdater::OTAUpdater(ConfigManager &config, DisplayManager &display) 
  : configManager(config), displayManager(display) {}

void OTAUpdater::setup() {
  ArduinoOTA.setPort(8266);
  if (strlen(configManager.getConfig()->otaPassword) > 0) {
    ArduinoOTA.setPassword(configManager.getConfig()->otaPassword);
  }

  ArduinoOTA.onStart([this]() {
    this->handleStart();
  });

  ArduinoOTA.onEnd([this]() {
    this->handleEnd();
  });

  ArduinoOTA.onError([this](ota_error_t error) {
    this->handleError(error);
  });
  MDNS.begin("nodemcu");
}

void OTAUpdater::handleStart() {
  displayManager.showOtaStartMessage();
}

void OTAUpdater::handleEnd() {
  displayManager.showOtaEndMessage();
  delay(2000);
}

void OTAUpdater::handleError(ota_error_t error) {
  displayManager.showOtaErrorMessage(error);
  delay(5000);
}

void OTAUpdater::handle() {
  ArduinoOTA.handle();
  MDNS.update();
}