#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#if defined(ESP8266)
  #include <ESP8266WebServer.h>
  #define WEB_SERVER_CLASS ESP8266WebServer
#elif defined(ESP32)
  #include <WebServer.h>
  #define WEB_SERVER_CLASS WebServer
  #include <Update.h>
#endif


#include "ConfigManager.h"
#include "DisplayManager.h"
#include "Variables.h"

class WebServerManager {
public:
  WebServerManager(ConfigManager &config, DisplayManager &display);
  void setup();
  void handle();

private:
  WEB_SERVER_CLASS server;
  ConfigManager &configManager;
  DisplayManager &displayManager;

  void handleRoot();
  void handleConfigGet();
  void handleConfigPost();
  void handleUpdateGet();
  void handleUpdatePost();
  String htmlEscape(const String &data);
  String htmlEscape(const char* data);
};

#endif
