#ifndef VARIABLES_H
#define VARIABLES_H

#define LED_BUILTIN 2
#define EEPROM_SIZE 512
#define CONFIG_MAGIC 0xDEADBEEF
#define OLED_SDA D2
#define OLED_SCL D1
#define SERVER_PORT 80
#define AP_SSID "TemperatureController"
#define AP_PASSWORD "PN123456"

enum AppState {
  MAIN_SCREEN,
  SETTINGS_SCREEN,
};

#endif