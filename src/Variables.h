#ifndef VARIABLES_H
#define VARIABLES_H
#include <ESP8266WiFi.h>

#define LED_BUILTIN 2
#define EEPROM_SIZE 1024
#define CONFIG_MAGIC 0xDEADBEEF
#define OLED_SDA D2
#define OLED_SCL D1
#define SERVER_PORT 80
#define AP_SSID "TemperatureController"
#define AP_PASSWORD "PN123456"

enum AppState {
  WIFI,
  MAIN_SCREEN,
  INFO_SCREEN,
  LOADING_SCREEN,
  STATES_COUNT,
};

enum WiFiStateMode {
  WIFI_MODE, AP_MODE
};

struct WiFiState {
  String name;
  String apPassword;
  IPAddress ip;
  WiFiStateMode mode;
};

struct GlobalState {
  float temperature;
  bool isRelayEnabled;
  WiFiState wifiState;
  AppState currentState; 
};

extern GlobalState globalState;

#endif