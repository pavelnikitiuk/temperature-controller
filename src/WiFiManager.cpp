#include "WiFiManager.h"

WiFiManager::WiFiManager(ConfigManager &config, DisplayManager &display)
    : configManager(config), displayManager(display) {}

bool WiFiManager::begin() {
  displayManager.setScreen(WIFI);
  connect();
  return true;
}

void WiFiManager::connect() {
  Serial.println(globalState.configuration.wifiState.name);
  Serial.println(globalState.configuration.wifiState.name);
  WiFi.mode(WIFI_STA);
#if defined(WOKWI)
  WiFi.begin("Wokwi-GUEST", "", 6);
  Serial.println("Connecting to wokwi guest network");
#else
  WiFi.begin(globalState.configuration.wifiState.name, globalState.configuration.wifiState.password);
#endif

  Serial.println("Connecting to WIFI network");
  startTime = millis();
  state = State::CONNECTING;
}

void WiFiManager::handle() {
  if (state == State::CONNECTING) {
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("\nConnected to WIFI");
      state = State::CONNECTED;
      globalState.view.currentState = MAIN_SCREEN;
    } else if (millis() - startTime >= timeout) {
      Serial.println("\nFailed to connect to WIFI network. Creating AP");
      startAP();
      globalState.view.currentState = MAIN_SCREEN;
      state = State::FAILED;
    } else {
      Serial.print(".");
    }
  }
}

void WiFiManager::startAP() {
  WiFi.mode(WIFI_AP);
  WiFi.softAPdisconnect(true);
  WiFi.softAP(AP_SSID, AP_PASSWORD);
  Serial.print("Access Point IP Address: ");
  Serial.println(WiFi.softAPIP());
}

WiFiState WiFiManager::getWiFiState() {
  WiFiState state;
  if (WiFi.status() == WL_CONNECTED) {
    state.mode = WIFI_MODE;
    state.ip = WiFi.localIP();
    strcpy(state.name, WiFi.SSID().c_str());
  } else {
    state.mode = AP_MODE;
    state.ip = WiFi.softAPIP();
    strcpy(state.name, AP_SSID);
    strcpy(state.apPassword, AP_PASSWORD);
  }
  return state;
}
