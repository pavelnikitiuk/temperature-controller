#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <U8g2lib.h>
#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>
#include <Wire.h>

#include "Variables.h"

class DisplayManager {
  public:
    DisplayManager();
    void showSplashScreen();
    void updateStatus();
    bool begin();
    void showOtaStartMessage();
    void showOtaEndMessage();
    void showOtaErrorMessage(ota_error_t error);
    void showOtaUpdateMessage();
    void showOtaRebootMessage();
  private:
    unsigned long lastUpdate;
    U8G2_SSD1306_128X64_NONAME_1_SW_I2C u8g2;
};

#endif