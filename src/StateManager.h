#ifndef STATE_MANAGER_H
#define STATE_MANAGER_H
#include "ConfigManager.h"
#include "DisplayManager.h"
#include "OTAUpdater.h"
#include "RelayManager.h"
#include "RotaryManager.h"
#include "StateObserver.h"
#include "TelegramManager.h"
#include "TemperatureManager.h"
#include "Variables.h"
#include "WiFiManager.h"
#include "CurrentManager.h"

const int viewScreensCount = 5;
class StateManager {
public:
  StateManager(ConfigManager &config, DisplayManager &display, OTAUpdater &ota,
               WiFiManager &wiFi, RotaryManager &rotary,
               TemperatureManager &temperature, RelayManager &relay,
               TelegramManager &telegram, CurrentManager &current);
  void handle();
  void updateGlobalState();
  bool begin();

private:
  ConfigManager &configManager;
  DisplayManager &displayManager;
  OTAUpdater &otaManager;
  WiFiManager &wiFiManager;
  StateObserver stateObserver;
  RotaryManager &rotaryManager;
  TemperatureManager &temperatureManager;
  RelayManager &relayManager;
  TelegramManager &telegramManager;
  CurrentManager &currentManager;
  unsigned long updateTime;
  unsigned long lastUpdateTime;
  void registerHandlers();
  AppState viewScreens[viewScreensCount];
  int findStateIndex(AppState state);
  void swithScreen(int screensCount);
  void setRelayStatus(bool status);
  void handleRelayModeChange();
};
#endif