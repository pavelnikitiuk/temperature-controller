#include "DisplayManager.h"

DisplayManager::DisplayManager() : u8g2(U8G2_R0, OLED_SCL, OLED_SDA, -1)
{
}

bool DisplayManager::begin()
{
  pinMode(OLED_SCL, OUTPUT);
  digitalWrite(OLED_SCL, LOW);
  delay(100);
  digitalWrite(OLED_SCL, HIGH);
  delay(500);  // Пауза для стабилизации питания

  // Инициализация I2C
  Wire.begin(OLED_SDA, OLED_SCL);
  Wire.setClockStretchLimit(150000);
  u8g2.begin();
  u8g2.setBusClock(100000);  // 100 kHz
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.setContrast(150);
  lastUpdate = millis();
  return true;
}

void DisplayManager::showSplashScreen()
{
  u8g2.firstPage();
  do
  {
    u8g2.drawStr(10, 20, "NodeMCU OTA");
    u8g2.drawStr(10, 40, "Version 1.0");
    u8g2.drawStr(10, 60, "Loading...");
  } while (u8g2.nextPage());
  delay(2000);
}

void DisplayManager::updateStatus()
{
  if (millis() - lastUpdate < 1000) {
    return;
  }
  lastUpdate = millis();
  u8g2.firstPage();
  do
  {
    u8g2.setCursor(0, 12);
    u8g2.print("WiFi: ");
    u8g2.print(WiFi.SSID());

    u8g2.setCursor(0, 24);
    u8g2.print("IP: ");
    u8g2.print(WiFi.localIP());

    u8g2.setCursor(0, 36);
    u8g2.print("Port ");
    u8g2.print(SERVER_PORT);

    u8g2.setCursor(0, 48);
    u8g2.print("Free RAM: ");
    u8g2.print(ESP.getFreeHeap());

    u8g2.drawHLine(0, 50, 128);
  } while (u8g2.nextPage());
}

void DisplayManager::showOtaStartMessage()
{
  u8g2.firstPage();
  do
  {
    u8g2.drawStr(0, 20, "OTA Update Start!");
    u8g2.drawStr(0, 40, "Please wait...");
  } while (u8g2.nextPage());
}


void DisplayManager::showOtaEndMessage()
{
  u8g2.firstPage();
  do {
    u8g2.drawStr(0, 20, "Update Complete!");
    u8g2.drawStr(0, 40, "Rebooting...");
  } while (u8g2.nextPage());
}

void DisplayManager::showOtaErrorMessage(ota_error_t error) {
  u8g2.firstPage();
  do {
    u8g2.setCursor(0, 20);
    u8g2.print("OTA Error: ");
    u8g2.print(error);
    u8g2.setCursor(0, 40);
    u8g2.print(error);
  } while (u8g2.nextPage());
}

void DisplayManager::showOtaUpdateMessage() {
  u8g2.firstPage();
  do
  {
    u8g2.drawStr(0, 20, "Firmware Update");
    u8g2.drawStr(0, 40, "Uploading...");
  } while (u8g2.nextPage());
}

void DisplayManager::showOtaRebootMessage() {
  u8g2.firstPage();
  do
  {
    u8g2.drawStr(0, 20, "Update Complete!");
    u8g2.drawStr(0, 40, "Rebooting...");
  } while (u8g2.nextPage());
}
