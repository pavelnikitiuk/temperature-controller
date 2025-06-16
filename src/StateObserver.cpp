#include "StateObserver.h"

void StateObserver::begin(const GlobalState &initialState) {
  previousState = initialState;
}

void StateObserver::onRelayChanged(std::function<void(bool)> callback) {
  relayChangedCallback = callback;
}

void StateObserver::onTemperatureChanged(std::function<void(float)> callback) {
  temperatureChangedCallback = callback;
}

void StateObserver::onAppStateChanged(std::function<void(AppState)> callback) {
  appStateChangedCallback = callback;
}

void StateObserver::onWiFiChanged(std::function<void(const WiFiState &)> callback) {
  wifiChangedCallback = callback;
}

void StateObserver::onRelayModeChangedCallback(std::function<void(const RelayControlMode &)> callback) {
  relayModeChangedCallback = callback;
}

void StateObserver::handle(const GlobalState &newState) {
  if (previousState.configuration.isRelayEnabled != newState.configuration.isRelayEnabled && relayChangedCallback) {
    relayChangedCallback(newState.configuration.isRelayEnabled);
  }

  if (previousState.view.temperature != newState.view.temperature && temperatureChangedCallback) {
    temperatureChangedCallback(newState.view.temperature);
  }

  if (previousState.view.currentState != newState.view.currentState && appStateChangedCallback) {
    appStateChangedCallback(newState.view.currentState);
  }

  if ((previousState.configuration.wifiState.mode != newState.configuration.wifiState.mode ||
       previousState.configuration.wifiState.ip != newState.configuration.wifiState.ip ||
       previousState.configuration.wifiState.name != newState.configuration.wifiState.name) && wifiChangedCallback) {
    wifiChangedCallback(newState.configuration.wifiState);
  }

  if(previousState.configuration.relayControl.mode != newState.configuration.relayControl.mode && relayModeChangedCallback) {
    relayModeChangedCallback(newState.configuration.relayControl.mode);
  }

  previousState = newState;
}
