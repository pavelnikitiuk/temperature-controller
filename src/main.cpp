#include <ESP8266WiFi.h>

#include "DisplayManager.h"
#include "ConfigManager.h"
#include "WebServer.h"
#include "WiFiManager.h"
#include "OTAUpdater.h"

DisplayManager display;
ConfigManager config;
WebServer webServer(config, display);
WiFiManager wifi(config);
OTAUpdater ota(config, display);


void setup()
{
  Serial.begin(115200);
  config.load();
  display.begin();
  display.showSplashScreen();
  wifi.begin();
  ota.setup();
  webServer.setup();
  // if (config.checkUpdates) {
  //   updateTicker.attach(config.updateInterval * 3600, checkForUpdates);
  // }
}

void loop()
{
  ota.handle();
  webServer.handleClient();
  display.updateStatus();
}

// void updateDisplay() {
//   u8g2.firstPage();
//   do {
//     u8g2.setCursor(0, 12);
//     u8g2.print("WiFi: ");
//     u8g2.print(WiFi.SSID());

//     u8g2.setCursor(0, 24);
//     u8g2.print("IP: ");
//     u8g2.print(WiFi.localIP());

//     u8g2.setCursor(0, 36);
//     u8g2.print("Port ");
//     u8g2.print(SERVER_PORT);

//     u8g2.setCursor(0, 48);
//     u8g2.print("Free RAM: ");
//     u8g2.print(ESP.getFreeHeap());

//     u8g2.drawHLine(0, 50, 128);
//   } while (u8g2.nextPage());
// }

// void displaySplashScreen() {
//   u8g2.firstPage();
//   do {
//     u8g2.drawStr(10, 20, "NodeMCU OTA");
//     u8g2.drawStr(10, 40, "Version 1.0");
//     u8g2.drawStr(10, 60, "Loading...");
//   } while (u8g2.nextPage());
//   delay(2000);
// }

// void loadConfig() {
//   EEPROM.get(0, config);
//   if (config.magic != CONFIG_MAGIC) {
//     resetConfig();
//   }
// }

// void resetConfig() {
//   config.magic = CONFIG_MAGIC;
//   strcpy(config.ssid, "");
//   strcpy(config.password, "");
//   strcpy(config.otaPassword, "admin");
//   config.checkUpdates = false;
//   config.updateInterval = 24;
//   saveConfig();
// }

// void saveConfig() {
//   EEPROM.put(0, config);
//   EEPROM.commit();
// }

// void startAPMode() {
//   WiFi.mode(WIFI_AP);
//   WiFi.softAP("NodeMCU-Config", "config1234");

//   Serial.println("\nAP Mode Activated");
//   Serial.print("SSID: NodeMCU-Config");
//   Serial.print("IP: ");
//   Serial.println(WiFi.softAPIP());

//   u8g2.firstPage();
//   do {
//     u8g2.drawStr(0, 20, "AP Mode Active");
//     u8g2.drawStr(0, 35, "SSID: NodeMCU-Config");
//     u8g2.drawStr(0, 50, "IP: 192.168.4.1");
//   } while (u8g2.nextPage());
// }

// void setupOTA() {
//   ArduinoOTA.setPort(8266);
//   if (strlen(config.otaPassword) > 0) {
//     ArduinoOTA.setPassword(config.otaPassword);
//   }

//   ArduinoOTA.onStart([]() {
//     u8g2.firstPage();
//     do {
//       u8g2.drawStr(0, 20, "OTA Update Start!");
//       u8g2.drawStr(0, 40, "Please wait...");
//     } while (u8g2.nextPage());
//   });

//   ArduinoOTA.onEnd([]() {
//     u8g2.firstPage();
//     do {
//       u8g2.drawStr(0, 20, "Update Complete!");
//       u8g2.drawStr(0, 40, "Rebooting...");
//     } while (u8g2.nextPage());
//     delay(2000);
//   });

//   ArduinoOTA.onError([](ota_error_t error) {
//     u8g2.firstPage();
//     do {
//       u8g2.setCursor(0, 20);
//       u8g2.print("OTA Error: ");
//       u8g2.print(error);
//       u8g2.setCursor(0, 40);
//       u8g2.print(error);
//     } while (u8g2.nextPage());
//     delay(5000);
//   });
// }

// void setupWebServer() {
//   server.on("/", handleRoot);
//   server.on("/config", HTTP_GET, []() {
//     String html = "<form method='POST' action='/config'>"
//                   "SSID: <input type='text' name='ssid' value='"+String(config.ssid)+"'><br>"
//                   "Password: <input type='password' name='pass'><br>"
//                   "OTA Password: <input type='text' name='otapass' value='"+String(config.otaPassword)+"'><br>"
//                   "Auto-update: <input type='checkbox' name='autoupdate' "+(config.checkUpdates?"checked":"")+"><br>"
//                   "Interval (hours): <input type='number' name='interval' value='"+String(config.updateInterval)+"'><br>"
//                   "<input type='submit'></form>";
//     server.send(200, "text/html", html);
//   });

//   server.on("/config", HTTP_POST, []() {
//     strlcpy(config.ssid, server.arg("ssid").c_str(), sizeof(config.ssid));
//     strlcpy(config.password, server.arg("pass").c_str(), sizeof(config.password));
//     strlcpy(config.otaPassword, server.arg("otapass").c_str(), sizeof(config.otaPassword));
//     config.checkUpdates = server.hasArg("autoupdate");
//     config.updateInterval = server.arg("interval").toInt();
//     saveConfig();
//     server.send(200, "text/plain", "Settings saved. Rebooting...");
//     delay(1000);
//     ESP.restart();
//   });

//   server.on("/update", HTTP_GET, []() {
//     if(!server.authenticate("admin", config.otaPassword)) {
//       return server.requestAuthentication();
//     }
//     String html = "<form method='POST' action='/update' enctype='multipart/form-data'>"
//                   "<input type='file' name='firmware'>"
//                   "<input type='submit' value='Update Firmware'>"
//                   "</form>";
//     server.send(200, "text/html", html);
//   });

//   server.on("/update", HTTP_POST, []() {
//     server.sendHeader("Connection", "close");
//     server.send(200, "text/plain", ESPhttpUpdate.getLastErrorString());
//   }, []() {
//     HTTPUpload& upload = server.upload();
//     if(upload.status == UPLOAD_FILE_START) {
//       u8g2.firstPage();
//       do {
//         u8g2.drawStr(0, 20, "Firmware Update");
//         u8g2.drawStr(0, 40, "Uploading...");
//       } while (u8g2.nextPage());

//       WiFiUDP::stopAll();
//       uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
//       if(!Update.begin(maxSketchSpace)) {
//         Update.printError(Serial);
//       }
//     } else if(upload.status == UPLOAD_FILE_WRITE) {
//       if(Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
//         Update.printError(Serial);
//       }
//     } else if(upload.status == UPLOAD_FILE_END) {
//       if(Update.end(true)) {
//         u8g2.firstPage();
//         do {
//           u8g2.drawStr(0, 20, "Update Complete!");
//           u8g2.drawStr(0, 40, "Rebooting...");
//         } while (u8g2.nextPage());
//         delay(2000);
//         ESP.restart();
//       } else {
//         Update.printError(Serial);
//       }
//     }
//   });
//   server.begin();
// }

// void checkForUpdates() {
//   Serial.println("Checking for updates...");
//   WiFiClient client;
//   HTTPClient http;

//   http.begin(client, "https://pavel.nikiti.uk/firmware.bin"); // Используем WiFiClient
//   int httpCode = http.GET();

//   if (httpCode == HTTP_CODE_OK) {
//     t_httpUpdate_return ret = ESPhttpUpdate.update(client, http.getString()); // Правильный вызов
//     // Альтернативный вариант:
//     // t_httpUpdate_return ret = ESPhttpUpdate.update("your-server.com", 80, "/firmware.bin");

//     switch(ret) {
//       case HTTP_UPDATE_FAILED:
//         Serial.printf("Update failed: %s\n", ESPhttpUpdate.getLastErrorString().c_str());
//         break;
//       case HTTP_UPDATE_NO_UPDATES:
//         Serial.println("No updates available");
//         break;
//       case HTTP_UPDATE_OK:
//         Serial.println("Update success");
//         break;
//     }
//   }
//   http.end();
// }