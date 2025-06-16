#include "WebServerManager.h"

WebServerManager::WebServerManager(ConfigManager &config, DisplayManager &display)
    : server(80), configManager(config), displayManager(display) {}

void WebServerManager::setup() {
  server.on("/", std::bind(&WebServerManager::handleRoot, this));
  server.on("/config", HTTP_GET, std::bind(&WebServerManager::handleConfigGet, this));
  server.on("/config", HTTP_POST, std::bind(&WebServerManager::handleConfigPost, this));
  server.on("/update", HTTP_GET, std::bind(&WebServerManager::handleUpdateGet, this));
  server.on("/update", HTTP_POST, std::bind(&WebServerManager::handleUpdatePost, this));
  server.begin();
}

void WebServerManager::handle() {
  server.handleClient();
}

void WebServerManager::handleRoot() {
  server.send(200, "text/html",
              "<h1>Node Configuration</h1>"
              "<a href='/config'>Configure Settings</a><br>"
              "<a href='/update'>OTA Update</a>");
}

void WebServerManager::handleConfigGet() {
  String html = "<form method='POST' action='/config'>"
                "SSID: <input type='text' name='ssid' value='" + String(globalState.configuration.wifiState.name) + "'><br>"
                "Password: <input type='password' name='pass'><br>"
                "OTA Password: <input type='text' name='otapass' value='" + String(globalState.configuration.otaPassword) + "'><br>"
                "<input type='submit'></form>";
  server.send(200, "text/html", html);
}

void WebServerManager::handleConfigPost() {
  strlcpy(globalState.configuration.wifiState.name, server.arg("ssid").c_str(), sizeof(globalState.configuration.wifiState.name));
  strlcpy(globalState.configuration.wifiState.password, server.arg("pass").c_str(), sizeof(globalState.configuration.wifiState.password));
  strlcpy(globalState.configuration.otaPassword, server.arg("otapass").c_str(), sizeof(globalState.configuration.otaPassword));

  configManager.save();
  server.send(200, "text/plain", "Settings saved. Rebooting...");
  delay(1000);
  ESP.restart();
}

void WebServerManager::handleUpdateGet() {
  if (!server.authenticate("admin", globalState.configuration.otaPassword)) {
    return server.requestAuthentication();
  }

  String html = "<form method='POST' action='/update' enctype='multipart/form-data'>"
                "<input type='file' name='firmware'>"
                "<input type='submit' value='Update Firmware'>"
                "</form>";
  server.send(200, "text/html", html);
}

void WebServerManager::handleUpdatePost() {
  HTTPUpload &upload = server.upload();

  if (upload.status == UPLOAD_FILE_START) {
    displayManager.showOtaUpdateMessage();

#if defined(ESP8266)
    WiFiUDP::stopAll();
#elif defined(ESP32)
    WiFiUDP().stop();
#endif

    uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
    if (!Update.begin(maxSketchSpace)) {
      Update.printError(Serial);
    }
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
      Update.printError(Serial);
    }
  } else if (upload.status == UPLOAD_FILE_END) {
    if (Update.end(true)) {
      displayManager.showOtaRebootMessage();
      delay(2000);
      ESP.restart();
    } else {
      Update.printError(Serial);
    }
  }
}
