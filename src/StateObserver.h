#ifndef STATE_OBSERVER_H
#define STATE_OBSERVER_H

#include "Variables.h"

class StateObserver {
public:
  void begin(const GlobalState &initialState);

  void onRelayChanged(std::function<void(bool)> callback);
  void onTemperatureChanged(std::function<void(float)> callback);
  void onAppStateChanged(std::function<void(AppState)> callback);
  void onWiFiChanged(std::function<void(const WiFiState &)> callback);
  void onRelayModeChangedCallback(std::function<void(const RelayControlMode &)> callback);

  void handle(const GlobalState &newState);

private:
  GlobalState previousState;

  std::function<void(bool)> relayChangedCallback = nullptr;
  std::function<void(float)> temperatureChangedCallback = nullptr;
  std::function<void(AppState)> appStateChangedCallback = nullptr;
  std::function<void(const WiFiState &)> wifiChangedCallback = nullptr;
  std::function<void(const RelayControlMode &)> relayModeChangedCallback = nullptr;
};

#endif
