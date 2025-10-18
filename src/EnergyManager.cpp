#include "EnergyManager.h"

EnergyManager::EnergyManager(CurrentManager &current,
                             unsigned long saveIntervalMs)
    : currentManager(current), lastSaveTime(0),
      lastUpdateTime(0), saveInterval(saveIntervalMs) {}

void EnergyManager::begin() {
  if (globalState.configuration.watts != globalState.configuration.watts) {
    globalState.configuration.watts = 0;
  }
  lastUpdateTime = millis();
  lastSaveTime = millis();
}

void EnergyManager::handle() {
  unsigned long now = millis();
  unsigned long delta = now - lastUpdateTime;
  if (delta < 1000)
    return;

  lastUpdateTime = now;

  float power = currentManager.getPower();
  accumulatedEnergy += power * (delta / 3600000.0f);

  if (now - lastSaveTime >= saveInterval) {
    lastSaveTime = now;
    globalState.configuration.watts += accumulatedEnergy;
    accumulatedEnergy = 0;
  }
}

void EnergyManager::reset() { globalState.configuration.watts = 0; }
