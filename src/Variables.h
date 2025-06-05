#ifndef VARIABLES_H
#define VARIABLES_H

#if defined(ESP8266)
  #include <ESP8266WiFi.h>
  #define OLED_SDA_PIN D2
  #define OLED_SCL_PIN D1
#elif defined(ESP32)
  #include <WiFi.h>
  #define OLED_SDA_PIN 21
  #define OLED_SCL_PIN 22
#endif

#define EEPROM_SIZE 1024
#define CONFIG_MAGIC 0xDEADBEEF

#define SERVER_PORT 80
#define AP_SSID "TemperatureController"
#define AP_PASSWORD "PN123456"

enum AppState {
  WIFI,
  MAIN_SCREEN,
  INFO_SCREEN,
  TEMPERATURE_SCREEN,
  LOADING_SCREEN,
  STATES_COUNT,
};

enum WiFiStateMode {
  WIFI_MODE,
  AP_MODE
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
