#ifndef VARIABLES_H
#define VARIABLES_H

#include "lang/lang.h"

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
#define CONFIG_MAGIC 0xDEA135

#define SERVER_PORT 80
#define AP_SSID "TemperatureController"
#define AP_PASSWORD "PN123456"
#define OTA_PASSWORD "admin"

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
  char name[32];
  char apPassword[32];
  char password[32];
  IPAddress ip;
  WiFiStateMode mode;

  bool operator==(const WiFiState &other) const {
    return name == other.name && apPassword == other.apPassword &&
           ip == other.ip && mode == other.mode && password == other.password;
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

struct TelegramBotSettings {
  char chatId[128];
  char token[64];

  bool operator==(const TelegramBotSettings &other) const {
    return (chatId == other.chatId) && (token == other.token);
  }

  bool operator!=(const TelegramBotSettings &other) const {
    return !(*this == other);
  }
};

struct GlobalViewState {
  AppState currentState;
  float temperature;

  bool operator==(const GlobalViewState &other) const {
    return (currentState == other.currentState) &&
           (temperature == other.temperature);
  }

  bool operator!=(const GlobalViewState &other) const {
    return !(*this == other);
  }
};

struct GlobalConfigurationState {
  bool isRelayEnabled;
  WiFiState wifiState;
  RelayControl relayControl;
  char otaPassword[32];
  uint32_t magic;
  TelegramBotSettings telegram;

  bool operator==(const GlobalConfigurationState &other) const {
    return (isRelayEnabled == other.isRelayEnabled) &&
           (wifiState == other.wifiState) &&
           (relayControl == other.relayControl) &&
           otaPassword == other.otaPassword && magic == other.magic &&
           telegram == other.telegram;
  }

  bool operator!=(const GlobalConfigurationState &other) const {
    return !(*this == other);
  }
};

struct GlobalState {
  GlobalViewState view;
  GlobalConfigurationState configuration;

  bool operator==(const GlobalState &other) const {
    return view.temperature == other.view.temperature &&
           configuration.isRelayEnabled == other.configuration.isRelayEnabled &&
           configuration.wifiState == other.configuration.wifiState &&
           view.currentState == other.view.currentState &&
           configuration.relayControl == other.configuration.relayControl;
  }

  bool operator!=(const GlobalState &other) const { return !(*this == other); }
};

extern GlobalState globalState;

#endif
