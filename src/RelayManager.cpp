#include "RelayManager.h"

RelayManager::RelayManager(uint8_t relayPin) { pin = relayPin; }

bool RelayManager::begin() {
  pinMode(pin, OUTPUT);
  return true;
}

void RelayManager::setStatus(bool status) {
  Serial.println(status);
  digitalWrite(pin, status);
}

void RelayManager::toggle() { setStatus(!digitalRead(pin)); }
