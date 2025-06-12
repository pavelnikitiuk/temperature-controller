#ifndef STATE_MANAGER_H
#define STATE_MANAGER_H
#include "ConfigManager.h"
#include "DisplayManager.h"
#include "OTAUpdater.h"
#include "RotaryManager.h"
#include "StateObserver.h"
#include "Variables.h"
#include "WiFiManager.h"
#include "TemperatureManager.h"
#include "RelayManager.h"

const int viewScreensCount = 4;
class StateManager {
public:
  StateManager(ConfigManager &config, DisplayManager &display, OTAUpdater &ota,
               WiFiManager &wiFi, RotaryManager &rotary, TemperatureManager &temperature, RelayManager &relay);
  void handle();
  void updateGlobalState();

private:
  ConfigManager &configManager;
  DisplayManager &displayManager;
  OTAUpdater &otaManager;
  WiFiManager &wiFiManager;
  StateObserver stateObserver;
  RotaryManager &rotaryManager;
  TemperatureManager &temperatureManager;
  RelayManager &relayManager;
  unsigned long updateTime;
  unsigned long lastUpdateTime;
  void registerHandlers();
  AppState viewScreens[viewScreensCount];
  int findStateIndex(AppState state);
  void swithScreen(int screensCount);
};
#endif