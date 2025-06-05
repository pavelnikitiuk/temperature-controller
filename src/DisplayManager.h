#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <U8g2lib.h>
#if defined(ESP8266)
  #include <ESP8266WiFi.h>
#elif defined(ESP32)
  #include <WiFi.h>
#endif
#include <ArduinoOTA.h>
#include <Wire.h>
#include <map>
#include <memory>


#include "Variables.h"
#include "screens/Screen.h"
#include "screens/AnimatedScreen.h"
#include "screens/LoadingScreen.h"
#include "screens/InfoScreen.h"
#include "screens/MainScreen.h"
#include "screens/WiFiScreen.h"
#include "screens/TemperatureScreen.h"

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
    U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2;
    std::map<AppState, std::function<Screen*()>> screenFactory;
    void createScreenFactory();

    Screen* currentScreen = nullptr;
};

#endif