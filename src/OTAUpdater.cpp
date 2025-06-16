#include "OTAUpdater.h"

OTAUpdater::OTAUpdater(ConfigManager &config, DisplayManager &display)
  : configManager(config), displayManager(display) {}

void OTAUpdater::setup() {
  ArduinoOTA.setPort(8266);

  if (strlen(globalState.configuration.otaPassword) > 0) {
    ArduinoOTA.setPassword(globalState.configuration.otaPassword);
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

#ifdef ESP8266
  MDNS.begin("device");
#elif defined(ESP32)
  if (!MDNS.begin("device")) {
    Serial.println("Error setting up MDNS responder!");
  }
#endif

  ArduinoOTA.begin();
}

void OTAUpdater::handleStart() {
  displayManager.showOtaStartMessage();
}

void OTAUpdater::handleEnd() {
  displayManager.showOtaEndMessage();
  delay(2000);
}

void OTAUpdater::handleError(ota_error_t error) {
  Serial.println(error);
  displayManager.showOtaErrorMessage(error);
  delay(5000);
}

void OTAUpdater::handle() {
  ArduinoOTA.handle();
#ifdef ESP8266
  MDNS.update();
#endif
}
