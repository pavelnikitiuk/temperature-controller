#include "WiFiManager.h"

WiFiManager::WiFiManager(ConfigManager &config, DisplayManager &display)
    : configManager(config), displayManager(display) {}

bool WiFiManager::begin() {
  displayManager.setScreen(WIFI);
  connect();
  return true;
}

void WiFiManager::onConnected(std::function<void()> callback) {
  onConnectedCallback = callback;
}

void WiFiManager::connect() {
  WiFi.mode(WIFI_STA);
  if (!globalState.configuration.wifiState.password) {
    startAP();
    state = State::FAILED;
    globalState.view.currentState = MAIN_SCREEN;

    return;
  }

#if defined(WOKWI)
  WiFi.begin("Wokwi-GUEST", "", 6);
  Serial.println("Connecting to wokwi guest network");
#else
  char pass[64];
  WiFi.begin(globalState.configuration.wifiState.name,
             globalState.configuration.wifiState.password);
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
      if(onConnectedCallback) {
        onConnectedCallback();
      }
      globalState.configuration.wifiState.mode = WIFI_MODE;
      globalState.view.currentState = MAIN_SCREEN;
    } else if (millis() - startTime >= timeout) {
      Serial.println("\nFailed to connect to WIFI network. Creating AP");
      startAP();
      globalState.view.currentState = MAIN_SCREEN;
      state = State::FAILED;
      globalState.configuration.wifiState.mode = AP_MODE;
      configManager.save();
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
  WiFiState wifiState = globalState.configuration.wifiState;
  if (state == State::CONNECTING) {
    return wifiState;
  }
  if (WiFi.status() == WL_CONNECTED) {
    wifiState.mode = WIFI_MODE;
    wifiState.ip = WiFi.localIP();
    strcpy(wifiState.name, WiFi.SSID().c_str());
  } else {
    wifiState.mode = AP_MODE;
    wifiState.ip = WiFi.softAPIP();
    strcpy(wifiState.name, AP_SSID);
    strcpy(wifiState.apPassword, AP_PASSWORD);
  }
  return wifiState;
}
