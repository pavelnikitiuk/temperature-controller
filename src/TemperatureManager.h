#ifndef TEMPERATURE_MANAGER_H
#define TEMPERATURE_MANAGER_H

#include <OneWire.h>
#include <DallasTemperature.h>
#include <NonBlockingDallas.h> 

class TemperatureManager {
public:
  TemperatureManager(uint8_t pin);
  void begin();
  void handle();
  float getTemperature() const;

private:
  static void handleTemperatureChange(int deviceIndex, int32_t temperatureRAW);
  static float lastTemperature;
  uint8_t pin;
  OneWire oneWire;
  DallasTemperature sensors;
  NonBlockingDallas temperatureSensors;
  int updateInterval = 5000;
};

#endif
