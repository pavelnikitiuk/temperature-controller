// WiFiManager.h
#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include "ConfigManager.h"
#include "DisplayManager.h"
#if defined(ESP8266)
  #include <ESP8266WiFi.h>
#elif defined(ESP32)
  #include <WiFi.h>
#endif

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