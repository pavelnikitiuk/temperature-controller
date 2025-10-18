#ifndef CURRENT_MAMAGER_H
#define CURRENT_MAMAGER_H

#include <ACS712.h>
#include <Arduino.h>

class CurrentManager {
public:
  CurrentManager(uint8_t pin);
  void handle();
  float getCurrent();
  float getPower();

private:
  ACS712 sensor;
  float voltage;
  unsigned long lastReadTime;
  float current;
  float power;
};

#endif