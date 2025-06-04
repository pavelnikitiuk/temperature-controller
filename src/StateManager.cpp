#include "StateManager.h"

StateManager::StateManager(ConfigManager &config, DisplayManager &display,
                           OTAUpdater &ota, WiFiManager &wiFi)
    : configManager(config), displayManager(display), otaManager(ota),
      wiFiManager(wiFi), stateObserver(), rotaryManager(D6, D7, D5), temperatureManager(D3) {
  viewScreens[0] = MAIN_SCREEN;
  viewScreens[1] = INFO_SCREEN;
  stateObserver.begin(globalState);
  temperatureManager.begin();
  registerHandlers();
}

void StateManager::handle() {
  unsigned long currentTime = millis();
  if (currentTime - lastUpdateTime >= updateTime) {
    lastUpdateTime = currentTime;
    updateGlobalState();
  }
  stateObserver.update(globalState);
  rotaryManager.update();
  temperatureManager.handle();
}

void StateManager::updateGlobalState() {
  globalState.wifiState = wiFiManager.getWiFiState();
  globalState.isRelayEnabled = false;
  globalState.temperature = temperatureManager.getTemperature();
}

void StateManager::registerHandlers() {
  stateObserver.onAppStateChanged(
      [this](AppState state) { displayManager.setScreen(state); });

  rotaryManager.onEvent(RIGHT, [this]() { swithScreen(1); });

  rotaryManager.onEvent(LEFT, [this]() { swithScreen(-1); });
  rotaryManager.onEvent(PRESS, []() { Serial.println("Press"); });
}

int StateManager::findStateIndex(AppState state) {
  for (int i = 0; i < viewScreensCount; i++) {
    if (viewScreens[i] == state) {
      return i;
    }
  }
  return -1;
}

void StateManager::swithScreen(int screensCount) {
  int index = findStateIndex(globalState.currentState);
  if (index == -1) {
    Serial.print("Unknown state ");
    Serial.println(globalState.currentState);
    return;
  }
  int nextIndex = (index + screensCount + viewScreensCount) % viewScreensCount;
  globalState.currentState = viewScreens[nextIndex];
}