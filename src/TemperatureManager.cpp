#include "TemperatureManager.h"

TemperatureManager::TemperatureManager(uint8_t pin)
  : pin(pin), oneWire(pin), sensors(&oneWire) {}

void TemperatureManager::begin() {
  sensors.begin();
}

void TemperatureManager::handle() {
  unsigned long now = millis();
  if (now - lastReadTime >= updateInterval) {
    sensors.requestTemperatures();
    lastTemperature = sensors.getTempCByIndex(0);
    lastReadTime = now;
  }
}

float TemperatureManager::getTemperature() const {
  return lastTemperature;
}
