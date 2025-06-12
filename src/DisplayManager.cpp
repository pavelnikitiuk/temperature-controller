#include "DisplayManager.h"

DisplayManager::DisplayManager() : u8g2(U8G2_R0, U8X8_PIN_NONE) {
  createScreenFactory();
  setScreen(WIFI);
}

bool DisplayManager::begin() {
  Wire.begin(OLED_SDA_PIN, OLED_SCL_PIN);
  Wire.setClock(400000);
  u8g2.begin();
  lastUpdate = millis();
  return true;
}

void DisplayManager::handle() { currentScreen->handle(); }

void DisplayManager::createScreenFactory() {
  screenFactory[WIFI] = [this]() { return new WiFiScreen(u8g2); };
  screenFactory[MAIN_SCREEN] = [this]() { return new MainScreen(u8g2); };
  screenFactory[INFO_SCREEN] = [this]() { return new InfoScreen(u8g2); };
  screenFactory[TEMPERATURE_SETTINGS_SCREEN] = [this]() { return new TemperatureSettingScreen(u8g2); };
  screenFactory[MODE_SETTINGS_SCREEN] = [this]() { return new ModeSettingScreen(u8g2); };
}

void DisplayManager::setScreen(AppState state) {
  if (currentScreen) {
    currentScreen->screenWillRemove();
    delete currentScreen;
    currentScreen = nullptr;
  }

  auto it = screenFactory.find(state);
  if (it != screenFactory.end()) {
    currentScreen = it->second();
  } else {
    currentScreen = screenFactory[MAIN_SCREEN]();
  }
}

// TODO: create ota screen
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