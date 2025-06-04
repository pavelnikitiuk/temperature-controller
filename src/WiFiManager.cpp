#include "WiFiManager.h"

WiFiManager::WiFiManager(ConfigManager &config, DisplayManager &display)
  : configManager(config), displayManager(display) {}

bool WiFiManager::begin() {
  displayManager.setScreen(WIFI);
  connect();
  return true;
}

void WiFiManager::connect() {
  Config* config = configManager.getConfig();
  WiFi.mode(WIFI_STA);
  WiFi.begin(config->ssid, config->password);
  Serial.println("Connecting to WIFI network");
  startTime = millis();
  state = State::CONNECTING;
}

void WiFiManager::handle() {
  if (state == State::CONNECTING) {
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("\nConnected to WIFI");
      state = State::CONNECTED;
      globalState.currentState = INFO_SCREEN;
    } else if (millis() - startTime >= timeout) {
      Serial.println("\nFailed to connect to WIFI network. Creating AP");
      startAP();
      globalState.currentState = INFO_SCREEN;
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
    state.name = WiFi.SSID();
  } else {
    state.mode = AP_MODE;
    state.ip = WiFi.softAPIP();
    state.name = AP_SSID;
    state.apPassword = AP_PASSWORD;
  }
  return state;
}
