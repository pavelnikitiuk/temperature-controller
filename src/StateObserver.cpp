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

void StateObserver::onOnTemperatureChangedCallback(std::function<void(const float &)> callback) {
  onTemperatureChangedCallback = callback;
}

void StateObserver::onOffTemperatureChangedCallback(std::function<void(const float &)> callback) {
  offTemperatureChangedCallback = callback;
}

void StateObserver::handle(const GlobalState &newState) {

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

  if (previousState.configuration.isRelayEnabled != newState.configuration.isRelayEnabled && relayChangedCallback) {
    relayChangedCallback(newState.configuration.isRelayEnabled);
  }

  if(previousState.configuration.relayControl.temperatureOn != newState.configuration.relayControl.temperatureOn) {
    onTemperatureChangedCallback(newState.configuration.relayControl.temperatureOn);
  }

  if(previousState.configuration.relayControl.temperatureOff != newState.configuration.relayControl.temperatureOff) {
    offTemperatureChangedCallback(newState.configuration.relayControl.temperatureOff);
  }

  previousState = newState;
}
