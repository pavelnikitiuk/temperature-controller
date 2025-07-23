#define ESP32 1
#if defined(ESP8266)
#include <ESP8266WiFi.h>
#elif defined(ESP32)
#include <WiFi.h>
#endif

#include "Variables.h"
GlobalState globalState;

#include "ConfigManager.h"
#include "DisplayManager.h"
#include "OTAUpdater.h"
#include "RelayManager.h"
#include "RotaryManager.h"
#include "StateManager.h"
#include "TelegramManager.h"
#include "TemperatureManager.h"
#include "WebServerManager.h"
#include "WiFiManager.h"

DisplayManager display;
ConfigManager config;
WebServerManager webServer(config, display);
WiFiManager wifi(config, display);
OTAUpdater ota(config, display);
RotaryManager rotary(ROTARY_CLK_PIN, ROTARY_DT_PIN, ROTARY_SW_PIN);
TemperatureManager temperature(TEMP_PIN);
RelayManager relay(RELAY_PIN);
TelegramManager telegram;
StateManager state(config, display, ota, wifi, rotary, temperature, relay, telegram);

void setup() {
  Serial.begin(115200);
  config.load();
  display.begin();
  wifi.begin();
  wifi.onConnected([]() {
    if (globalState.configuration.telegram.token &&
        globalState.configuration.telegram.chatId) {
      telegram.begin(globalState.configuration.telegram.token,
                     globalState.configuration.telegram.chatId);
    }
  });
  ota.setup();
  webServer.setup();
  relay.begin();
}

#if defined(SHOW_FPS)
unsigned long lastMillis = millis();
int frames = 0;
#endif

void loop() {
  ota.handle();
  wifi.handle();
  webServer.handle();
  display.handle();
  state.handle();
  rotary.handle();
  temperature.handle();
  telegram.handle();

#if defined(SHOW_FPS)
  frames++;
  if (millis() - lastMillis >= 1000) {
    Serial.print("FPS: ");
    Serial.println(frames);
    frames = 0;
    lastMillis = millis();
  }
#endif
}