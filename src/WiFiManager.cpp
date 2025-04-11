#include "WiFiManager.h"

WiFiManager::WiFiManager(ConfigManager &config) 
  : configManager(config) {}

bool WiFiManager::begin() {
  bool isWifiConnected = connect();

  if (!isWifiConnected) {
    Serial.println("Failed to connect to WIFI network. Createing AP");
    startAP();
  } else
  {
    Serial.println("Connected to WIFI");
  }
  
  return true;
}

bool WiFiManager::connect() {
  Config* confing = configManager.getConfig();
  Serial.println(confing->ssid);
  Serial.println(confing->password);
  WiFi.mode(WIFI_STA);
  WiFi.begin(configManager.getConfig()->ssid, configManager.getConfig()->password);
  Serial.println("Connecting to WIFI network");
  unsigned long startTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startTime < 30000) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  return WiFi.status() == WL_CONNECTED;
}

void WiFiManager::startAP() {
  WiFi.mode(WIFI_AP);
  WiFi.softAPdisconnect(true);
  WiFi.softAP(AP_SSID, AP_PASSWORD);
  delay(1000);
  Serial.print("Access Point IP Address:");
  Serial.println(WiFi.softAPIP());
}