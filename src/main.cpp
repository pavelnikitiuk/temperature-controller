#define ESP32 1
#if defined(ESP8266)
  #include <ESP8266WiFi.h>
#elif defined(ESP32)
  #include <WiFi.h>
#endif

#include "Variables.h"
GlobalState globalState;

#include "DisplayManager.h"
#include "ConfigManager.h"
#include "WebServerManager.h"
#include "WiFiManager.h"
#include "OTAUpdater.h"
#include "StateManager.h"

DisplayManager display;
ConfigManager config;
WebServerManager webServer(config, display);
WiFiManager wifi(config, display);
OTAUpdater ota(config, display);
StateManager state(config, display, ota, wifi);

void setup()
{
  Serial.begin(115200);
  config.load();
  display.begin();
  wifi.begin();
  ota.setup();
  webServer.setup();
}
unsigned long lastMillis = millis();
int frames = 0;

void loop()
{
  ota.handle();
  wifi.handle();
  webServer.handle();
  display.handle();
  state.handle();

  frames++;
  if (millis() - lastMillis >= 1000) {
    Serial.print("FPS: ");
    Serial.println(frames);
    frames = 0;
    lastMillis = millis();
  }
}