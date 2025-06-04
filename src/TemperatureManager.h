#ifndef TEMPERATURE_MANAGER_H
#define TEMPERATURE_MANAGER_H

#include <OneWire.h>
#include <DallasTemperature.h>

class TemperatureManager {
public:
  TemperatureManager(uint8_t pin);
  void begin();
  void handle();
  float getTemperature() const;

private:
  uint8_t pin;
  OneWire oneWire;
  DallasTemperature sensors;
  float lastTemperature = 0.0;
  unsigned long lastReadTime = 0;
  const unsigned long updateInterval = 5000;
};

#endif
