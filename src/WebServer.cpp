#include "WebServer.h"

WebServer::WebServer(ConfigManager &config, DisplayManager &display)
    : server(80), configManager(config), displayManager(display) {}

void WebServer::setup()
{
  server.on("/", std::bind(&WebServer::handleRoot, this));
  server.on("/config", HTTP_GET, std::bind(&WebServer::handleConfigGet, this));
  server.on("/config", HTTP_POST, std::bind(&WebServer::handleConfigPost, this));
  server.on("/update", HTTP_GET, std::bind(&WebServer::handleUpdateGet, this));
  server.on("/update", HTTP_POST, [this]()
            { server.send(200, "text/plain", ESPhttpUpdate.getLastErrorString()); }, std::bind(&WebServer::handleConfigPost, this));
  server.begin();
}

void WebServer::handleClient()
{
  server.handleClient();
}

void WebServer::handleRoot()
{
  server.send(200, "text/html",
              "<h1>NodeMCU Configuration</h1>"
              "<a href='/config'>Configure Settings</a><br>"
              "<a href='/update'>OTA Update</a>");
}

void WebServer::handleConfigGet()
{
  Config *config = configManager.getConfig();
  String html = "<form method='POST' action='/config'>"
                "SSID: <input type='text' name='ssid' value='" +
                String(config->ssid) + "'><br>"
                                       "Password: <input type='password' name='pass'><br>"
                                       "OTA Password: <input type='text' name='otapass' value='" +
                String(config->otaPassword) + "'><br>"
                                              "Auto-update: <input type='checkbox' name='autoupdate' " +
                (config->checkUpdates ? "checked" : "") + "><br>"
                                                          "Interval (hours): <input type='number' name='interval' value='" +
                String(config->updateInterval) + "'><br>"
                                                 "<input type='submit'></form>";
  server.send(200, "text/html", html);
}

void WebServer::handleConfigPost()
{
  Config *config = configManager.getConfig();
  strlcpy(config->ssid, server.arg("ssid").c_str(), sizeof(config->ssid));
  strlcpy(config->password, server.arg("pass").c_str(), sizeof(config->password));
  strlcpy(config->otaPassword, server.arg("otapass").c_str(), sizeof(config->otaPassword));
  config->checkUpdates = server.hasArg("autoupdate");
  config->updateInterval = server.arg("interval").toInt();
  configManager.save();
  server.send(200, "text/plain", "Settings saved. Rebooting...");
  delay(1000);
  ESP.restart();
}

void WebServer::handleUpdateGet()
{
  Config *config = configManager.getConfig();
  if (!server.authenticate("admin", config->otaPassword))
  {
    return server.requestAuthentication();
  }
  String html = "<form method='POST' action='/update' enctype='multipart/form-data'>"
                "<input type='file' name='firmware'>"
                "<input type='submit' value='Update Firmware'>"
                "</form>";
  server.send(200, "text/html", html);
}

void WebServer::handleUpdatePost()
{
  HTTPUpload &upload = server.upload();
  if (upload.status == UPLOAD_FILE_START)
  {
    displayManager.showOtaUpdateMessage();

    WiFiUDP::stopAll();
    uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
    if (!Update.begin(maxSketchSpace))
    {
      Update.printError(Serial);
    }
  }
  else if (upload.status == UPLOAD_FILE_WRITE)
  {
    if (Update.write(upload.buf, upload.currentSize) != upload.currentSize)
    {
      Update.printError(Serial);
    }
  }
  else if (upload.status == UPLOAD_FILE_END)
  {
    if (Update.end(true))
    {
      displayManager.showOtaRebootMessage();
      delay(2000);
      ESP.restart();
    }
    else
    {
      Update.printError(Serial);
    }
  }
}