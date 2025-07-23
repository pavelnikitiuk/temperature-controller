#include "ModeSettingScreen.h"

void ModeSettingScreen::drawScreen() {
  bool isAuto = globalState.configuration.relayControl.mode == RELAY_CONTROL_AUTO;

  u8g2.drawFrame(0, 0, 128, 64);
  u8g2.drawHLine(0, 14, 128);

  u8g2.setFont(I18N_FONT_TITLE);
  u8g2.setCursor(4, 11);
  u8g2.print(I18N_SCREEN_MODE_TITLE);

  const char *modeStr = isAuto ? I18N_SCREEN_MODE_AUTO : I18N_SCREEN_MODE_MANUAL;
  const uint8_t *font = I18N_FONT_MODE_TEXT;

  u8g2.setFont(font);
  int16_t textWidth = u8g2.getUTF8Width(modeStr);
  int16_t x = (128 - textWidth) / 2;
  u8g2.setCursor(x, 50);
  u8g2.print(modeStr);
}

void ModeSettingScreen::onClick() {
  if (globalState.configuration.relayControl.mode == RELAY_CONTROL_MANUAL) {
    globalState.configuration.relayControl.mode = RELAY_CONTROL_AUTO;
  } else {
    globalState.configuration.relayControl.mode = RELAY_CONTROL_MANUAL;
  } 
}

unsigned int ModeSettingScreen::getUpdateTime() { return 200; }
