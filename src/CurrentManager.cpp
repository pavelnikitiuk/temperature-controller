#include "CurrentManager.h"

CurrentManager::CurrentManager(uint8_t pin)
    : sensor(pin, 3.3), voltage(220), lastReadTime(0), current(0), power(0) {}

void CurrentManager::handle() {
  unsigned long now = millis();
  if (now - lastReadTime < 1000)
    return;
  lastReadTime = now;

  current = sensor.mA_AC() / 1000;
  if (current < 0.05) {
    current = 0;
  }
  power = current * voltage;
}

float CurrentManager::getCurrent() { return current; }

float CurrentManager::getPower() { return power; }
