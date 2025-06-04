// WiFiManager.h
#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include "ConfigManager.h"
#include "DisplayManager.h"
#include <ESP8266WiFi.h>

class WiFiManager {
public:
  WiFiManager(ConfigManager &config, DisplayManager &display);
  bool begin();
  void connect();
  void startAP();
  void handle();
  WiFiState getWiFiState();

private:
  ConfigManager &configManager;
  DisplayManager &displayManager;
  enum class State { IDLE, CONNECTING, CONNECTED, FAILED };
  State state = State::IDLE;
  unsigned long startTime = 0;
  const unsigned long timeout = 30000;
};
#endif