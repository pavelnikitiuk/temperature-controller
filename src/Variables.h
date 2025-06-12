#ifndef VARIABLES_H
#define VARIABLES_H

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#define OLED_SDA_PIN D2
#define OLED_SCL_PIN D1
#define ROTARY_CLK_PIN D7
#define ROTARY_DT_PIN D6
#define ROTARY_SW_PIN D5
#define TEMP_PIN D3
#define RELAY_PIN D4
#elif defined(ESP32)
#include <WiFi.h>
#define OLED_SDA_PIN 21
#define OLED_SCL_PIN 22
#define ROTARY_CLK_PIN 18
#define ROTARY_DT_PIN 19
#define ROTARY_SW_PIN 21
#define TEMP_PIN 4
#define RELAY_PIN 2

#endif

#define EEPROM_SIZE 1024
#define CONFIG_MAGIC 0xDEADBEEF

#define SERVER_PORT 80
#define AP_SSID "TemperatureController"
#define AP_PASSWORD "PN123456"

#define OLED_WIDTH 128
#define OLED_HEIGHT 64

enum AppState {
  WIFI,
  MAIN_SCREEN,
  INFO_SCREEN,
  TEMPERATURE_SETTINGS_SCREEN,
  MODE_SETTINGS_SCREEN,
  LOADING_SCREEN,
  STATES_COUNT,
};

enum WiFiStateMode { WIFI_MODE, AP_MODE };
enum RelayControlMode { RELAY_CONTROL_MANUAL, RELAY_CONTROL_AUTO };

struct WiFiState {
  String name;
  String apPassword;
  IPAddress ip;
  WiFiStateMode mode;

  bool operator==(const WiFiState &other) const {
    return name == other.name && apPassword == other.apPassword &&
           ip == other.ip && mode == other.mode;
  }

  bool operator!=(const WiFiState &other) const { return !(*this == other); }
};

struct RelayControl {
  RelayControlMode mode;
  float temperatureOn;
  float temperatureOff;

  bool operator==(const RelayControl &other) const {
    return mode == other.mode && temperatureOn == other.temperatureOn &&
           temperatureOff == other.temperatureOff;
  }

  bool operator!=(const RelayControl &other) const { return !(*this == other); }
};

struct GlobalState {
  float temperature;
  bool isRelayEnabled;
  WiFiState wifiState;
  AppState currentState;
  RelayControl relayControl;

  bool operator==(const GlobalState &other) const {
    return temperature == other.temperature &&
           isRelayEnabled == other.isRelayEnabled &&
           wifiState == other.wifiState && currentState == other.currentState &&
           relayControl == other.relayControl;
  }

  bool operator!=(const GlobalState &other) const { return !(*this == other); }
};

extern GlobalState globalState;

#endif
