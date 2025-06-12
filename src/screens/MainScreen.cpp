#include "MainScreen.h"

void MainScreen::drawScreen() {
  currentState.isRelayEnabled = globalState.isRelayEnabled;
  currentState.temperature = globalState.temperature;

  u8g2.drawFrame(0, 0, 128, 64);
  u8g2.drawVLine(63, 0, 64);
  u8g2.drawVLine(64, 0, 64);

  u8g2.setFont(u8g2_font_6x10_tr);
  u8g2.drawStr(4, 12, "TEMP");
  u8g2.drawStr(68, 12, "RELAY");

  char tempStr[6];
  snprintf(tempStr, sizeof(tempStr), "%.1f", globalState.temperature);

  const uint8_t *fonts[] = {u8g2_font_logisoso24_tf, u8g2_font_logisoso22_tf,
                            u8g2_font_logisoso18_tf, u8g2_font_logisoso16_tf};

  int16_t tempWidth = 0;
  int16_t tempX = 0;
  const uint8_t *selectedFont =
      fonts[sizeof(fonts) / sizeof(fonts[0]) - 1];

  for (uint8_t i = 0; i < sizeof(fonts) / sizeof(fonts[0]); i++) {
    u8g2.setFont(fonts[i]);
    tempWidth = u8g2.getStrWidth(tempStr);
    if (tempWidth <= 64) {
      selectedFont = fonts[i];
      break;
    }
  }

  u8g2.setFont(selectedFont);
  tempX = (64 - tempWidth) / 2;
  u8g2.setCursor(tempX, 50);
  u8g2.print(tempStr);

  u8g2.setFont(u8g2_font_logisoso24_tf);
  const char *relayStr = globalState.isRelayEnabled ? "ON" : "OFF";
  int16_t relayWidth = u8g2.getStrWidth(relayStr);
  int16_t relayX = 64 + (64 - relayWidth) / 2;
  u8g2.setCursor(relayX, 50);
  u8g2.print(relayStr);
}

bool MainScreen::shouldUpdate() {
  return globalState.temperature != currentState.temperature ||
         globalState.isRelayEnabled != currentState.isRelayEnabled;
}

void MainScreen::onClick() {
  globalState.isRelayEnabled = !globalState.isRelayEnabled;
}