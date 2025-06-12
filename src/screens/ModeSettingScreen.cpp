#include "ModeSettingScreen.h"

void ModeSettingScreen::drawScreen() {
  bool isAuto = globalState.relayControl.mode == RELAY_CONTROL_AUTO;

  u8g2.drawFrame(0, 0, 128, 64);
  u8g2.drawHLine(0, 14, 128);

  u8g2.setFont(u8g2_font_6x10_tr);
  u8g2.drawStr(4, 11, "MODE");

  const char *modeStr = isAuto ? "AUTO" : "MANUAL";
  const uint8_t *font = u8g2_font_logisoso24_tf;

  u8g2.setFont(font);
  int16_t textWidth = u8g2.getStrWidth(modeStr);
  int16_t x = (128 - textWidth) / 2;
  u8g2.setCursor(x, 50);
  u8g2.print(modeStr);
}

bool ModeSettingScreen::shouldUpdate() { return true; }

void ModeSettingScreen::onClick() {
  if (globalState.relayControl.mode == RELAY_CONTROL_MANUAL) {
    globalState.relayControl.mode = RELAY_CONTROL_AUTO;
  } else {
    globalState.relayControl.mode = RELAY_CONTROL_MANUAL;
  }
}

unsigned int ModeSettingScreen::getUpdateTime() { return 200; }
