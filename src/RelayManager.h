#ifndef RELAY_MANAGER_H
#define RELAY_MANAGER_H

#include "Variables.h"

class RelayManager {
public:
  RelayManager(uint8_t pin);
  bool begin();
  void toggle();
  void setStatus(bool status);
private:
  uint8_t pin;
};

#endif
