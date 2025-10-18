#ifndef ENERGY_MANAGER_H
#define ENERGY_MANAGER_H

#include <Arduino.h>
#include <EEPROM.h>
#include "Variables.h"
#include "CurrentManager.h"

class EnergyManager {
public:
  EnergyManager(CurrentManager& currentManager, unsigned long saveIntervalMs = 60000);

  void begin();
  void handle();

  void reset();

private:
  CurrentManager& currentManager;
  float accumulatedEnergy;
  unsigned long lastUpdateTime;
  unsigned long lastSaveTime;
  unsigned long saveInterval;
};

#endif
