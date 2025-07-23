#include "StateManager.h"

StateManager::StateManager(ConfigManager &config, DisplayManager &display,
                           OTAUpdater &ota, WiFiManager &wifi,
                           RotaryManager &rotary,
                           TemperatureManager &temperature, RelayManager &relay,
                           TelegramManager &telegram)
    : configManager(config), displayManager(display), otaManager(ota),
      wiFiManager(wifi), rotaryManager(rotary), temperatureManager(temperature),
      relayManager(relay), telegramManager(telegram) {

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
  stateObserver.onTemperatureChanged([this](float currentTemperature) {
    RelayControl relayControl = globalState.configuration.relayControl;
    if (relayControl.mode != RELAY_CONTROL_AUTO) {
      return;
    }

    if (currentTemperature <= relayControl.temperatureOn &&
        !globalState.configuration.isRelayEnabled) {
      globalState.configuration.isRelayEnabled = true;
    }

    if (currentTemperature >= relayControl.temperatureOff &&
        globalState.configuration.isRelayEnabled) {
      globalState.configuration.isRelayEnabled = false;
    }
  });
  stateObserver.onAppStateChanged(
      [this](AppState state) { displayManager.setScreen(state); });

  stateObserver.onRelayChanged([this](bool value) { setRelayStatus(value); });

  stateObserver.onRelayModeChangedCallback(
      [this](RelayControlMode mode) { handleRelayModeChange(); });
  stateObserver.onOnTemperatureChangedCallback([this](float temperature) {
    telegramManager.sendTemperatureOnChanged(temperature);
    configManager.save();
  });
  stateObserver.onOffTemperatureChangedCallback([this](float temperature) {
    telegramManager.sendTemperatureOffChanged(temperature);
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
  rotaryManager.onEvent(CLICK,
                        [this]() { displayManager.currentScreen->onClick(); });
  rotaryManager.onEvent(HOLD,
                        [this]() { displayManager.currentScreen->onHold(); });

  telegramManager.onMessage(TELEGRAM_TEMPERATURE, [this]() {
    telegramManager.sendTemperature(globalState.view.temperature);
  });

  telegramManager.onMessage(TELEGRAM_ENABLE, [this]() {
    bool isRelayEnabled = globalState.configuration.isRelayEnabled;
    RelayControlMode mode = globalState.configuration.relayControl.mode;

    if (isRelayEnabled) {
      telegramManager.sendMessage(I18N_TELEGRAM_RELAY_ALREADY_ENABLED);
    } else if (mode == RELAY_CONTROL_AUTO) {
      telegramManager.sendMessage(I18N_TELEGRAM_AUTO_MODE_ENABLED);
    } else {
      globalState.configuration.isRelayEnabled = true;
    }
  });

  telegramManager.onMessage(TELEGRAM_DISABLE, [this]() {
    bool isRelayEnabled = globalState.configuration.isRelayEnabled;
    RelayControlMode mode = globalState.configuration.relayControl.mode;

    if (!isRelayEnabled) {
      telegramManager.sendMessage(I18N_TELEGRAM_RELAY_ALREADY_DISABLED);
    } else if (mode == RELAY_CONTROL_AUTO) {
      telegramManager.sendMessage(I18N_TELEGRAM_AUTO_MODE_ENABLED);
    } else {
      globalState.configuration.isRelayEnabled = false;
    }
  });
  telegramManager.onMessage(TELEGRAM_MODE, [this]() {
    RelayControlMode newMode;
    if (globalState.configuration.relayControl.mode == RELAY_CONTROL_MANUAL) {
      newMode = RELAY_CONTROL_AUTO;
    } else {
      newMode = RELAY_CONTROL_AUTO;
    }
    globalState.configuration.relayControl.mode = newMode;
  });

  telegramManager.onMessage(TELEGRAM_SHOW_SETTINGS, [this]() {
    float on = globalState.configuration.relayControl.temperatureOn;
    float off = globalState.configuration.relayControl.temperatureOff;
    telegramManager.sendShowSettings(on, off);
  });
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

void StateManager::setRelayStatus(bool status) {
  relayManager.setStatus(status);
  telegramManager.sendRelayStateChanged(status);
}

void StateManager::handleRelayModeChange() {
  configManager.save();
  telegramManager.sendRelayModeChanged(
      globalState.configuration.relayControl.mode);
}