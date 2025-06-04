#include <ESP8266WiFi.h>

#include "Variables.h"
GlobalState globalState;

#include "DisplayManager.h"
#include "ConfigManager.h"
#include "WebServer.h"
#include "WiFiManager.h"
#include "OTAUpdater.h"
#include "StateManager.h"

DisplayManager display;
ConfigManager config;
WebServer webServer(config, display);
WiFiManager wifi(config, display);
OTAUpdater ota(config, display);
StateManager state(config, display, ota, wifi);

void setup()
{
  Serial.begin(115200);
  config.load();
  Serial.println(config.getConfig()->password);
  display.begin();
  wifi.begin();
  ota.setup();
  webServer.setup();
}

void loop()
{
  ota.handle();
  wifi.handle();
  webServer.handle();
  display.handle();
  state.handle();
}