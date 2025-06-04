#include "DisplayManager.h"

DisplayManager::DisplayManager()
    : u8g2(U8G2_R0, OLED_SCL, OLED_SDA, -1), mainScreen(u8g2),
      loadingScreen(u8g2), wiFiScreen(u8g2), infoScreen(u8g2),
      currentScreen(&wiFiScreen), previousScreen(nullptr) {}

bool DisplayManager::begin() {
  pinMode(OLED_SCL, OUTPUT);
  digitalWrite(OLED_SCL, LOW);
  delay(100);
  digitalWrite(OLED_SCL, HIGH);
  delay(500);

  Wire.begin(OLED_SDA, OLED_SCL);
  Wire.setClockStretchLimit(150000);
  u8g2.begin();
  u8g2.setBusClock(100000);
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.setContrast(150);
  lastUpdate = millis();
  return true;
}

void DisplayManager::handle() {
  bool isScreenChanged = currentScreen != previousScreen;
  currentScreen->handle(isScreenChanged);
 }

void DisplayManager::updateStatus() {
  if (millis() - lastUpdate < 1000) {
    return;
  }
  lastUpdate = millis();
  setScreen(MAIN_SCREEN);
}

void DisplayManager::showOtaStartMessage() {
  u8g2.firstPage();
  do {
    u8g2.drawStr(0, 20, "OTA Update Start!");
    u8g2.drawStr(0, 40, "Please wait...");
  } while (u8g2.nextPage());
}

void DisplayManager::showOtaEndMessage() {
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
  delay(1000);
}

void DisplayManager::showOtaUpdateMessage() {
  u8g2.firstPage();
  do {
    u8g2.drawStr(0, 20, "Firmware Update");
    u8g2.drawStr(0, 40, "Uploading...");
  } while (u8g2.nextPage());
}

void DisplayManager::showOtaRebootMessage() {
  u8g2.firstPage();
  do {
    u8g2.drawStr(0, 20, "Update Complete!");
    u8g2.drawStr(0, 40, "Rebooting...");
  } while (u8g2.nextPage());
}

Screen &DisplayManager::getScreenForState(AppState state) {
  switch (state) {
  case WIFI:
    return wiFiScreen;
  case MAIN_SCREEN:
    return mainScreen;
  case LOADING_SCREEN:
    return loadingScreen;
  case INFO_SCREEN:
    return infoScreen;

  default:
    return mainScreen;
  }
}

void DisplayManager::setScreen(AppState state) {
  previousScreen = currentScreen;
  Screen &screen = getScreenForState(state);
  currentScreen = &screen;
}
