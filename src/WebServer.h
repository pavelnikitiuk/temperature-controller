// WebServer.h
#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <ESP8266WebServer.h>
#include <ESP8266httpUpdate.h>
#include "ConfigManager.h"
#include "DisplayManager.h"

class WebServer {
  public:
    WebServer(ConfigManager &config, DisplayManager &display);
    void setup();
    void handleClient();

  private:
    ESP8266WebServer server;
    ConfigManager &configManager;
    DisplayManager &displayManager;
    void handleRoot();
    void handleConfigGet();
    void handleConfigPost();
    void handleUpdateGet();
    void handleUpdatePost();
};

#endif