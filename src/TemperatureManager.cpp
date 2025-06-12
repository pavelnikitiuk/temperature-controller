#include "TemperatureManager.h"

float TemperatureManager::lastTemperature = -10.0;

TemperatureManager::TemperatureManager(uint8_t pin)
  : pin(pin), oneWire(pin), sensors(&oneWire), temperatureSensors(&sensors) {}

void TemperatureManager::begin() {
  temperatureSensors.begin(NonBlockingDallas::resolution_9, updateInterval);
  temperatureSensors.onTemperatureChange(handleTemperatureChange);
}

void TemperatureManager::handle() {
  temperatureSensors.update();
}

float TemperatureManager::getTemperature() const {
  return lastTemperature;
}

void TemperatureManager::handleTemperatureChange(int deviceIndex, int32_t temperatureRAW) {
  lastTemperature = temperatureRAW * 0.0078125f;
}