#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <U8g2lib.h>
#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>
#include <Wire.h>

#include "Variables.h"
#include "screens/Screen.h"
#include "screens/AnimatedScreen.h"
#include "screens/LoadingScreen.h"
#include "screens/InfoScreen.h"
#include "screens/MainScreen.h"
#include "screens/WiFiScreen.h"

class DisplayManager {
  public:
    DisplayManager();
    void handle();
    void updateStatus();
    bool begin();
    void showOtaStartMessage();
    void showOtaEndMessage();
    void showOtaErrorMessage(ota_error_t error);
    void showOtaUpdateMessage();
    void showOtaRebootMessage();
    void (*getDrawFunctionForState(AppState state))();

    void setScreen(AppState state);
  private:
    unsigned long lastUpdate;
    Screen& getScreenForState(AppState state);
    U8G2_SSD1306_128X64_NONAME_1_SW_I2C u8g2;
    MainScreen mainScreen;
    LoadingScreen loadingScreen;
    WiFiScreen wiFiScreen;
    InfoScreen infoScreen;
    Screen* currentScreen;
    Screen* previousScreen;
};

#endif