#include "MainScreen.h"

void MainScreen::drawScreen() {
  u8g2.drawFrame(0, 0, 128, 64);
  u8g2.drawVLine(63, 0, 64);
  u8g2.drawVLine(64, 0, 64);

  u8g2.setFont(I18N_FONT_TITLE);
  u8g2.drawUTF8(4, 12, I18N_SCREEN_TEMPERATURE_TITLE);
  u8g2.drawUTF8(68, 12, I18N_SCREEN_STATE_TITLE);

  char tempStr[6];
  snprintf(tempStr, sizeof(tempStr), "%.1f", globalState.view.temperature);

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
  const char *relayStr = globalState.configuration.isRelayEnabled ? "ON" : "OFF";
  int16_t relayWidth = u8g2.getStrWidth(relayStr);
  int16_t relayX = 64 + (64 - relayWidth) / 2;
  u8g2.setCursor(relayX, 50);
  u8g2.print(relayStr);
}

void MainScreen::onClick() {
  if (globalState.configuration.relayControl.mode != RELAY_CONTROL_MANUAL) {
    return;
  }
  globalState.configuration.isRelayEnabled = !globalState.configuration.isRelayEnabled;
}