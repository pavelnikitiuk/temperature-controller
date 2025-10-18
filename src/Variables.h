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
#define CURRENT_PIN A0
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
#include <ArduinoJson.h>

#define EEPROM_SIZE 1024
#define CONFIG_MAGIC 0xDEA135

#define SERVER_PORT 80
#define AP_SSID "TemperatureController"
#define AP_PASSWORD "PN123456"
#define OTA_PASSWORD "admin"

#define OLED_WIDTH 128
#define OLED_HEIGHT 64

#define CURRENT_SENSITIVITY 0.100

enum AppState {
  WIFI,
  MAIN_SCREEN,
  INFO_SCREEN,
  TEMPERATURE_SETTINGS_SCREEN,
  MODE_SETTINGS_SCREEN,
  POWER_SCREEN,
  STATES_COUNT,
};

enum TelegramSettingsMenuState {
  TELEGRAM_SETTINGS_MENU_HIDDEN,
  TELEGRAM_SETTINGS_ASK_TEMPERATURE_TYPE,
  TELEGRAM_SETTINGS_ASK_TEMPERATURE_ON,
  TELEGRAM_SETTINGS_ASK_TEMPERATURE_OFF,
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

  void toJson(ArduinoJson::V742PB22::JsonObject &doc) const {
    doc["name"] = name;
    doc["apPassword"] = apPassword;
    doc["password"] = password;
    doc["ip"] = ip.toString();
    doc["mode"] = mode;
  }
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

  void toJson(ArduinoJson::V742PB22::JsonObject &doc) const {
    doc["mode"] = mode;
    doc["temperatureOn"] = temperatureOn;
    doc["temperatureOff"] = temperatureOff;
  }
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

  void toJson(ArduinoJson::V742PB22::JsonObject &doc) const {
    doc["chatId"] = chatId;
    doc["token"] = token;
  }
};

struct TelegramSettingsMenu {
  int messageId;
  TelegramSettingsMenuState state;

  void toJson(ArduinoJson::V742PB22::JsonObject &doc) const {
    doc["messageId"] = messageId;
    doc["state"] = state;
  }
};

struct GlobalViewState {
  AppState currentState;
  float temperature;
  float power;
  float current;

  bool operator==(const GlobalViewState &other) const {
    return (currentState == other.currentState) &&
           (temperature == other.temperature) && (power == other.power) &&
           (current && other.current);
  }

  bool operator!=(const GlobalViewState &other) const {
    return !(*this == other);
  }

  void toJson(ArduinoJson::V742PB22::JsonObject &doc) const {
    doc["currentState"] = currentState;
    doc["temperature"] = temperature;
  }
};

struct GlobalConfigurationState {
  bool isRelayEnabled;
  WiFiState wifiState;
  RelayControl relayControl;
  char otaPassword[32];
  uint32_t magic;
  TelegramBotSettings telegram;
  TelegramSettingsMenu telegramSettingsMenu;
  float watts;

  bool operator==(const GlobalConfigurationState &other) const {
    return (isRelayEnabled == other.isRelayEnabled) &&
           (wifiState == other.wifiState) &&
           (relayControl == other.relayControl) &&
           otaPassword == other.otaPassword && magic == other.magic &&
           telegram == other.telegram && watts == other.watts;
  }

  bool operator!=(const GlobalConfigurationState &other) const {
    return !(*this == other);
  }

  void toJson(ArduinoJson::V742PB22::JsonObject &doc) const {
    doc["isRelayEnabled"] = isRelayEnabled;
    JsonObject wifi = doc.createNestedObject("wifiState");
    wifiState.toJson(wifi);

    JsonObject relay = doc.createNestedObject("relayControl");
    relayControl.toJson(relay);

    doc["otaPassword"] = otaPassword;
    doc["magic"] = magic;

    JsonObject bot = doc.createNestedObject("telegram");
    telegram.toJson(bot);
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

  void toJson(ArduinoJson::V742PB22::JsonObject &doc) const {
    JsonObject viewObj = doc.createNestedObject("view");
    view.toJson(viewObj);

    JsonObject configObj = doc.createNestedObject("configuration");
    configuration.toJson(configObj);
  }
};

extern GlobalState globalState;

#define TELEGRAM_EXIT_BUTTON "button1"
#define TELEGRAM_CHANGE_ON_BUTTON "button2"
#define TELEGRAM_CHANGE_OFF_BUTTON "button3"

#endif
