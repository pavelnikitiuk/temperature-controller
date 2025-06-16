#include "StateManager.h"

StateManager::StateManager(ConfigManager &config, DisplayManager &display,
                           OTAUpdater &ota, WiFiManager &wifi,
                           RotaryManager &rotary,
                           TemperatureManager &temperature, RelayManager &relay)
    : configManager(config), displayManager(display), otaManager(ota),
      wiFiManager(wifi), rotaryManager(rotary), temperatureManager(temperature),
      relayManager(relay) {

  viewScreens[0] = MAIN_SCREEN;
  viewScreens[1] = TEMPERATURE_SETTINGS_SCREEN;
  viewScreens[2] = MODE_SETTINGS_SCREEN;
  viewScreens[3] = INFO_SCREEN;

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
  stateObserver.handle(globalState);
}

void StateManager::updateGlobalState() {
  globalState.configuration.wifiState = wiFiManager.getWiFiState();
  globalState.view.temperature = temperatureManager.getTemperature();
}

void StateManager::registerHandlers() {
  stateObserver.onAppStateChanged(
      [this](AppState state) { displayManager.setScreen(state); });

  stateObserver.onRelayChanged([this](bool value) {
    relayManager.setStatus(value);
  });

  stateObserver.onRelayModeChangedCallback([this](RelayControlMode mode) {
    configManager.save();
  });

  rotaryManager.onEvent(RIGHT, [this]() {
    bool shouldSwithScreen = displayManager.currentScreen->onRightScroll();
    if (shouldSwithScreen) {
      swithScreen(1);
    }
  });
  rotaryManager.onEvent(LEFT, [this]() {
    bool shouldSwithScreen = displayManager.currentScreen->onLeftScroll();
    if (shouldSwithScreen) {
      swithScreen(-1);
    }
  });
  rotaryManager.onEvent(CLICK, [this]() {
    displayManager.currentScreen->onClick();
  });
  rotaryManager.onEvent(HOLD,
                        [this]() { displayManager.currentScreen->onHold(); });
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
  int index = findStateIndex(globalState.view.currentState);
  if (index == -1) {
    Serial.print("Unknown state ");
    Serial.println(globalState.view.currentState);
    return;
  }
  int nextIndex = (index + screensCount + viewScreensCount) % viewScreensCount;
  globalState.view.currentState = viewScreens[nextIndex];
}
