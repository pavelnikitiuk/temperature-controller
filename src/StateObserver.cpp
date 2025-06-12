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

void StateObserver::handle(const GlobalState &newState) {
  if (previousState.isRelayEnabled != newState.isRelayEnabled && relayChangedCallback) {
    relayChangedCallback(newState.isRelayEnabled);
  }

  if (previousState.temperature != newState.temperature && temperatureChangedCallback) {
    temperatureChangedCallback(newState.temperature);
  }

  if (previousState.currentState != newState.currentState && appStateChangedCallback) {
    appStateChangedCallback(newState.currentState);
  }

  if ((previousState.wifiState.mode != newState.wifiState.mode ||
       previousState.wifiState.ip != newState.wifiState.ip ||
       previousState.wifiState.name != newState.wifiState.name) && wifiChangedCallback) {
    wifiChangedCallback(newState.wifiState);
  }

  previousState = newState;
}
